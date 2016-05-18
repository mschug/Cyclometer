/*
 * PushButtonDetection.cpp
 *
 *  Created on: May 17, 2016
 *      Author: ssj3000
 */

#include "PushButtonDetection.h"


PushButtonDetection::PushButtonDetection(){

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
	daio_portA_handle   = mmap_device_io(PORT_LENGTH, BASE_ADDRESS + DAIO_PORTA_ADDRESS);

	// Configure DAIO Port A as Input Port by default
	// -- output port - 0
	// -- input port - 1
	// daio_ctrl_handle = mmap_device_io(PORT_LENGTH, BASE_ADDRESS + DAIO_CONTROLREG_ADDRESS);
	int val = in8(daio_ctrl_handle);
	out8(daio_ctrl_handle, 0x80 | val);

	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);		// initialize thread attributes structure
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

	// Start the thread
	pthread_create(	&PushButtonDetectionThreadID,
					&threadAttr,
					&PushButtonDetection::PushButtonDetectionThread,
					this);
}


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
	// 1. MODE + SET + START/STOP : Pressed for >= 2 seconds --> FULL RESET
	// 2. MODE                    : Toggle between km and miles
	// 3. SET                     : Stores mode and circumference values
	// 4. START/STOP              : Starting and Stopping cyclometer
	// -----------------------------------------------------------------------

	//Signal signal = NO_SIGNAL;

	while(true)
	{
		std::cout << "PushButtonDetection::PushButtonDetectionThread" << std::endl;
		

		// Read DAIO A2, A3 and A4 pins only
		//  A2 - MODE
		//  A3 - SET
		//  A4 - START/STOP
		int pbVal_last = in8(self->daio_portA_handle) 
						& 0b00111000;

		int startTime = gblCounter;

		switch(pb_val)
		{
			case 0b00111000: // 1. MODE+SET+START/STOP : >=2 secs : FULL RESET
				
				// start timer for 2 secs and check if the values toggle 
				// from 1 to 0 after 2 secs
				while((gblCounter-startTime) <= 2000)
				{
					int pbVal_curr = in8(self->daio_portA_handle) & 0b00111000;
					if(pbVal_last != pbVal_curr)
					{
						curr_signal = NO_SIGNAL;
						break;
					}
					else if(pbVal_last == pbVal_curr)
					{
						curr_signal = ALL_HELD;
					}
				}
				
				break;


			case 0b00100000: // 2. MODE : Toggle between km and miles
				
				while((gblCounter-startTime) <= 2000)
				{
					int pbVal_curr = in8(self->daio_portA_handle) & 0b00100000;
					if(pbVal_last != pbVal_curr)
					{
						curr_signal = MODE;
						break;
					}
					else if(pbVal_last == pbVal_curr)
					{
						curr_signal = MODE_HELD;
					}
				}

				break;


			case 0b00010000: // 3. SET : Stores mode and circumference values
				
				curr_signal = SET;
				
				break;


			case 0b00001000: // 4. START/STOP: Starting and Stopping cyclometer
				
				if(self->lastStartStop == STOP)
				{
					curr_signal = START;
				}
				else if(self->lastStartStop == START)
				{
					curr_signal = STOP;
				}
				
				break;
		}

	}
	return 0;
}
