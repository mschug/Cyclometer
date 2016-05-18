/*
 * DisplayOperations.cpp
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

DisplayOperations::DisplayOperations(StateContext* state_machine)
{
  threadAlive = false;
  m_display_mode = KILOMETERS;
  m_circumference = 210; // Stored in centimeters
  m_state_machine = state_machine;

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

static void* DisplayOperations::DisplayOperationsThread(void* arg)
{
  threadAlive = true;

  while(true)
  {
    StateEnum state = m_state_machine->getDisplayState();
    switch(state)
    {
      case SET_UNITS:
        break;
      case SET_TIRE_CIRC:
        break;
      case CYCLE_TIRE_CIRC:
        break;
      case DISPLAY_DATA:
        StateEnum internal_state = m_state_machine->getDisplayStateInternal();
        switch(internal_state)
        {
          case DISPLAY_SPEED:
            break;
          case DISPLAY_DISTANCE:
            break;
          case DISPLAY_TIME:
            break;
        }
        break;
      case INVALID_STATE:
      default:
        break;
    }

    usleep(1000);
  }
}

DisplayOperations::DisplayOperations(){}

DisplayOperations::~DisplayOperations()
{
  threadAlive = false;
}
