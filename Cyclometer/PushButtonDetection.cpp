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
		std::cout << "PushButtonDetection::PushButtonDetectionThread" << std::endl;
		
		// Read DAIO A2, A3 and A4 pins only
		//  DAIO A2 - MODE
		//  DAIO A3 - SET
		//  DAIO A4 - START/STOP
		pbVal_last = in8(self->daio_portA_handle) & 0b00111000;

		// Know the initial time for further calculations
		startTime = gblCounter;
		// std::cerr << "PushButtonDetection::PushButtonDetectionThread: startTime:  " << startTime << std::endl;

		// Poll for 200msec wait for detecting simultaneous button events
		while((gblCounter - startTime) <= PBSIMULPRESS_TIMEOUT)
		{
			//std::cout << "PushButtonDetection::PushButtonDetectionThread: gblCounter:  " << gblCounter << std::endl;
			pbVal_curr = in8(self->daio_portA_handle) & 0b00111000;
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
				break;

			case 0b00111000: // 1. MODE+SET+START/STOP : >=2 secs : FULL RESET
				
				// start timer for 2 secs and check if the values toggle 
				// from 1 to 0 after 2 secs
				while((gblCounter-startTime) <= PBPRESS_TIMEOUT)
				{
					pbVal_curr = in8(self->daio_portA_handle) & 0b00111000;

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


			case 0b00100000:
				// 2. MODE : Toggle between km and miles
				// 3. MODE HELD : Increment the circumference
				while((gblCounter-startTime) <= PBPRESS_TIMEOUT)
				{
					pbVal_curr = in8(self->daio_portA_handle) & 0b00111000;
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


			case 0b00010000:
				// 4. SET : Stores mode and circumference values
				curr_signal = SET;
				
				break;


			case 0b00001000:
				// 5. START/STOP : Starting and Stopping cyclometer
				if(self->lastStartStop == STOP)
				{
					curr_signal = START;
				}
				else if(self->lastStartStop == START)
				{
					curr_signal = STOP;
				}
				
				break;


			case 0b00101000:
				// 6. MODE+START/STOP : for 2 seconds -->reset the trip values
				while((gblCounter-startTime) <= PBPRESS_TIMEOUT)
				{
					pbVal_curr = in8(self->daio_portA_handle) & 0b00111000;
					if(pbVal_last != pbVal_curr)
					{
						curr_signal = NO_SIGNAL;
						break;
					}
					else if(pbVal_last == pbVal_curr)
					{
						curr_signal = MODE_START_HELD;
					}
				}

				break;
		}
		std::cout << "PushButtonDetection::PushButtonDetectionThread: curr_signal: " << curr_signal << std::endl;

	}
	return 0;
}
