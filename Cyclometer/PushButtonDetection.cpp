/*
 * PushButtonDetection.cpp
 *
 *  Created on: May 17, 2016
 *      Author: ssj3000
 */

#include "PushButtonDetection.h"


PushButtonDetection::PushButtonDetection(StateContext* state_machine, DisplayOperations* display, InputCalculation* calculation){

	m_state_machine = state_machine;
	m_display = display;
	m_calculation = calculation;
	lastStartStop = STOP;

	// Some initializations
	int privity_err;

	/* Give this thread root permissions to access the hardware */
	privity_err = ThreadCtl(_NTO_TCTL_IO, NULL);

	if (privity_err == -1) {
		std::cout <<"can't get root permissions" << std::endl;
		return;
	}

	// Get a handle to the DAIO port's registers
	daio_ctrl_handle    = mmap_device_io(PORT_LENGTH, BASE_ADDRESS + DAIO_CONTROLREG_ADDRESS);
	daio_portC_handle   = mmap_device_io(PORT_LENGTH, BASE_ADDRESS + DAIO_PORTC_ADDRESS);

	// Configure DAIO Port C as Input Port by default
	// -- output port - 0
	// -- input port - 1
	int val = in8(daio_ctrl_handle);
	out8(daio_ctrl_handle, 0x01);// | val); // DIRCL

	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);		// initialize thread attributes structure
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

	// Start the thread
	pthread_create(	&PushButtonDetectionThreadID,
					&threadAttr,
					&PushButtonDetection::PushButtonDetectionThread,
					this);
}

PushButtonDetection::PushButtonDetection(){}

PushButtonDetection::~PushButtonDetection() {

	// Some uninitializations
	pthread_join(PushButtonDetectionThreadID, NULL);
}


void* PushButtonDetection::PushButtonDetectionThread(void* arg)
{
    // Give this thread root permissions to access the hardware
    int privity_err = ThreadCtl(_NTO_TCTL_IO, NULL);
    if (privity_err == -1)
    {
        std::cout <<"can't get root permissions" << std::endl;
        return NULL;
    }

	PushButtonDetection* self = (PushButtonDetection*)arg;

	// -----------------------------------------------------------------------
	// Pushbuttons events:
	// -----------------------------------------------------------------------
	// 1. ALL HELD                : Pressed for >= 2 seconds --> FULL RESET
	// 2. MODE                    : Toggle between km and miles
	// 3. MODE HELD               : Increment the circumference
	// 4. SET                     : Stores mode and circumference values
	// 5. START/STOP              : Starting and Stopping cyclometer
	// 6. MODE+START/STOP         : reset the trip values
	// -----------------------------------------------------------------------

	//Signal signal = NO_SIGNAL;
	int pbVal_last(0), pbVal_curr(0);
	int startTime(0);

	while(true)
	{
		std::string msg = "NO_SIGNAL";
		//std::cout << "PushButtonDetection::PushButtonDetectionThread" << std::endl;
		
		// Read DAIO C0, C1 and C2 pins only
		//  DAIO C0 - MODE
		//  DAIO C1 - SET
		//  DAIO C2 - START/STOP
		pbVal_last = in8(self->daio_portC_handle);// & 0b00000111;
		//std::cerr << pbVal_last << std::endl;
		int val = in8(self->daio_ctrl_handle);
		//std::cerr << val << std::endl;

		// Know the initial time for further calculations
		startTime = gblCounter;
		// std::cerr << "PushButtonDetection::PushButtonDetectionThread: startTime:  " << startTime << std::endl;

		// Poll for 200msec wait for detecting simultaneous button events
		while((gblCounter - startTime) <= PBSIMULPRESS_TIMEOUT)
		{
			//std::cout << "PushButtonDetection::PushButtonDetectionThread: gblCounter:  " << gblCounter << std::endl;
			pbVal_curr = in8(self->daio_portC_handle) & 0b00000111;
		}

		// std::cerr << "PushButtonDetection::PushButtonDetectionThread: 200msec done!" << std::endl;

		if(pbVal_last != pbVal_curr)
		{
			pbVal_last = pbVal_curr;
		}


		switch( pbVal_last )
		{
			case 0b00000000: // None pressed or all released
				curr_signal = NO_SIGNAL;
				msg = "NO_SIGNAL";

				if (last_signal == MODE_HELD || last_signal == MODE)
				{
					curr_signal = MODE_RELEASED;
					msg = "MODE_RELEASED";
				}

				break;

			case 0b00000111: // 1. MODE+SET+START/STOP : >=2 secs : FULL RESET
				
				// start timer for 2 secs and check if the values toggle 
				// from 1 to 0 after 2 secs
				while((gblCounter-startTime) <= PBPRESS_TIMEOUT)
				{
					pbVal_curr = in8(self->daio_portC_handle) & 0b00000111;

					if(pbVal_last != pbVal_curr)
					{
						curr_signal = NO_SIGNAL;
						msg = "NO_SIGNAL";
						break;
					}
					else if(pbVal_last == pbVal_curr)
					{
						curr_signal = ALL_HELD;
						msg = "ALL_HELD";
					}
				}
				
				break;


			case 0b00000001:
				// 2. MODE : Toggle between km and miles
				// 3. MODE HELD : Increment the circumference
				while((gblCounter-startTime) <= PBPRESS_TIMEOUT)
				{
					pbVal_curr = in8(self->daio_portC_handle) & 0b00000111;
					if(pbVal_last != pbVal_curr)
					{
						curr_signal = MODE;
						msg = "MODE";

						if (self->m_state_machine->getDisplayState() == SET_UNITS)
						{
							self->m_display->changeMode();
						}
						else if (self->m_state_machine->getDisplayState() == SET_TIRE_CIRC)
						{
							self->m_display->cycleCircumference();
						}
						break;
					}
					else if(pbVal_last == pbVal_curr)
					{
						curr_signal = MODE_HELD;
						msg = "MODE_HELD";
					}
				}

				break;


			case 0b00000010:
				// 4. SET : Stores mode and circumference values
				curr_signal = SET;
				msg = "SET";
				
				if (self->m_state_machine->getDisplayStateInternal() == DISPLAY_DISTANCE ||
					self->m_state_machine->getDisplayStateInternal() == DISPLAY_TIME)
				{
					curr_signal = CHANGE_CALC_MODE;
					msg = "CHANGE_CALC_MODE";
				}
				else if (self->m_state_machine->getDisplayStateInternal() == DISPLAY_SPEED)
				{
					curr_signal = CHANGE_TIRE_CIRC;
					msg = "CHANGE_TIRE_CIRC";
				}

				break;


			case 0b00000100:
				// 5. START/STOP : Starting and Stopping cyclometer
				if(self->lastStartStop == STOP || self->lastStartStop == STOP)
				{
					curr_signal = START;
					msg = "START";
				}
				else if(self->lastStartStop == START)
				{
					curr_signal = STOP;
					msg = "STOP";
				}
				
				break;


			case 0b00000101:
				// 6. MODE+START/STOP : for 2 seconds -->reset the trip values
				while((gblCounter-startTime) <= PBPRESS_TIMEOUT)
				{
					pbVal_curr = in8(self->daio_portC_handle) & 0b00000111;
					if(pbVal_last != pbVal_curr)
					{
						curr_signal = NO_SIGNAL;
						msg = "NO_SIGNAL";
						break;
					}
					else if(pbVal_last == pbVal_curr)
					{
						curr_signal = MODE_START_HELD;
						msg = "MODE_START_HELD";

						if (self->m_state_machine->getDisplayState() == DISPLAY_DATA)
						{
							self->m_calculation->resetTripValues();
						}
					}
				}

				break;
		}
		//std::cerr << "PushButtonDetection::PushButtonDetectionThread: " <<
		//		"curr_signal: " << msg << std::endl;
		self->m_state_machine->acceptSignal(curr_signal);
		last_signal = curr_signal;
	}
	return 0;
}
