/*
 * DisplayOperations.cpp
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

#include "DisplayOperations.h"

DisplayOperations::DisplayOperations(StateContext* state_machine)
{
  threadAlive = false;
  m_display_mode = KILOMETERS;
  m_circumference = 210; // Stored in centimeters
  m_state_machine = state_machine;
  m_speed = 0;
  m_distance = 0;
  m_time = 0;

  // Initialize thread
	int privity_err;

	/* Give this thread root permissions to access the hardware */
	privity_err = ThreadCtl(_NTO_TCTL_IO, NULL);

	if (privity_err == -1) {
		std::cout <<"can't get root permissions" << std::endl;
		return;
	}

  // Get a handle to the DAIO port's registers
  daio_ctrl_handle    = mmap_device_io(PORT_LENGTH, BASE_ADDRESS + DAIO_CONTROLREG_ADDRESS);
  daio_portA_handle   = mmap_device_io(PORT_LENGTH, BASE_ADDRESS + DAIO_PORTA_ADDRESS);
  daio_portB_handle   = mmap_device_io(PORT_LENGTH, BASE_ADDRESS + DAIO_PORTB_ADDRESS);

  // Configure DAIO Ports A and B as output by default
  // Port B uses control pin 4 (fifth bit from the right), A uses control pin 1
  int val = in8(daio_ctrl_handle);
  out8(daio_ctrl_handle, (0b11101101 & val));

  pthread_attr_t threadAttr;
  pthread_attr_init(&threadAttr);		// initialize thread attributes structure
  pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

  pthread_create(	&DisplayOperationsThreadID,
          &threadAttr,
          &DisplayOperations::DisplayOperationsThread,
          this);
}

void* DisplayOperations::DisplayOperationsThread(void* arg)
{
	DisplayOperations* self = (DisplayOperations*) arg;
  self->threadAlive = true;
  unsigned int output[4];
  std::cout << "DisplayOperations::DisplayOperationsThread" << endl;

  while(true)
  {
    StateEnum state = self->m_state_machine->getDisplayState();
    StateEnum internal_state = self->m_state_machine->getDisplayStateInternal();

    switch(state)
    {
      case SET_UNITS:
        // Output three blank displays with 1 for km, 2 for miles
        output[0] = BLANK;
        output[1] = BLANK;
        output[2] = BLANK;
        if (self->m_display_mode == KILOMETERS)
          output[3] = ONE;
        else
          output[3] = TWO;

        self->sendOutput(output);
        break;

      case SET_TIRE_CIRC:
      case CYCLE_TIRE_CIRC:
        // Output current circumference
    	  self->numberToOutput(output, self->m_circumference);
    	  self->sendOutput(output);
        break;

      case DISPLAY_DATA:
        if(internal_state == DISPLAY_SPEED)
        {
        	self->numberToOutput(output, self->m_speed);

			for (int i = 0; i < 3; i++)
			{
			  if (output[i] == BLANK) output[i] = ZERO;
			}

			if ((self->m_speed/100000) % 10 == 1 || self->m_speed == 0) // Current is < 10
				output[0] &= DECIMAL;
			if ((self->m_speed/10000)  % 10 == 1 || self->m_speed == 0) // Average is < 10
				output[2] &= DECIMAL;

			output[1] &= DECIMAL;
			self->sendOutput(output);
        }
        else if(internal_state == DISPLAY_DISTANCE)
        {
        	self->numberToOutput(output, self->m_distance);
          if (self->m_distance < 10)
            output[2] = ZERO;
          output[2] &= DECIMAL;
          self->sendOutput(output);
        }
        else if(internal_state == DISPLAY_TIME)
        {
        	self->numberToOutput(output, self->m_time);
          for (int i = 0; i < 3; i++)
          {
            if (output[i] == BLANK) output[i] = ZERO;
          }
          output[1] &= DECIMAL;
          self->sendOutput(output);
        }
        break;

      case INVALID_STATE:
      default:
        output[0] = DASH;
        output[1] = DASH;
        output[2] = DASH;
        output[3] = DASH;
        self->sendOutput(output);
        break;
    }
  }

  return 0;
}

/* Converts the last four digits of a number into a set of SevenSegment outputs.
 * Decimal places will be handled by the DisplayOperationsThread. */
void DisplayOperations::numberToOutput(unsigned int* output, unsigned int num)
{
  for (int i = 3; i >= 0; i--)
  {
    // Store last digit in rightmost array position
    int last_digit = num % 10;
    switch(last_digit)
    {
      case 0:
        output[i] = ZERO;
        if (num == 0 && i != 3)
        {
          output[i] = BLANK;
        }
        break;
      case 1:
        output[i] = ONE;
        break;
      case 2:
        output[i] = TWO;
        break;
      case 3:
        output[i] = THREE;
        break;
      case 4:
        output[i] = FOUR;
        break;
      case 5:
        output[i] = FIVE;
        break;
      case 6:
        output[i] = SIX;
        break;
      case 7:
        output[i] = SEVEN;
        break;
      case 8:
        output[i] = EIGHT;
        break;
      case 9:
        output[i] = NINE;
        break;
    }

    num /= 10; // Remove last digit
  }
}

/* Outputs the seven segment display values one at a time. */
void DisplayOperations::sendOutput(unsigned int* output)
{
  int val = in8(daio_portB_handle) | SELECT_DISPLAY;
  out8(daio_portB_handle, val & FIRST_DISPLAY); // Set anode 3 low
  out8(daio_portA_handle, output[0]);
  usleep(100);

  val = in8(daio_portB_handle) | SELECT_DISPLAY; // in8 duplicated to remain thread-safe
  out8(daio_portB_handle, val & SECOND_DISPLAY); // Set anode 2 low
  out8(daio_portA_handle, output[1]);
  usleep(100);

  val = in8(daio_portB_handle) | SELECT_DISPLAY;
  out8(daio_portB_handle, val & THIRD_DISPLAY); // Set anode 1 low
  out8(daio_portA_handle, output[2]);
  usleep(100);

  val = in8(daio_portB_handle) | SELECT_DISPLAY;
  out8(daio_portB_handle, val & FOURTH_DISPLAY); // Set anode 0 low
  out8(daio_portA_handle, output[3]);
  usleep(100);
}

DisplayOperations::DisplayOperations(){}

DisplayOperations::~DisplayOperations()
{
  threadAlive = false;
}
