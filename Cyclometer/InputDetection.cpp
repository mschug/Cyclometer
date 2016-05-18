/*
 * InputDetection.cpp
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#include "InputDetection.h"


InputDetection::InputDetection(){

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
	pthread_create(	&InputDetectionThreadID, 
					&threadAttr, 
					&InputDetection::InputDetectionThread, 
					this);
}


InputDetection::~InputDetection() {

	// Some uninitializations
	pthread_join(InputDetectionThreadID, NULL);
}


void* InputDetection::InputDetectionThread(void* arg)
{
    // Give this thread root permissions to access the hardware
    int privity_err = ThreadCtl(_NTO_TCTL_IO, NULL);
    if (privity_err == -1) {
        std::cout <<"can't get root permissions" << std::endl;
        return NULL;
    }

	//bool inputPulseDetected = false;
	watchdogFlag = UNKNOWN_SIGNAL;

	while(true)
	{
		std::cout << "InputDetection::InputDetectionThread" << std::endl;
		// check only the PIN connected to pulse generator
		while( ( in8(((InputDetection*)arg)->daio_portA_handle) && 0x01) == 1)
		{
			// Reset watchdog
			watchdogFlag = START_WATCHDOG;
			// std::cout << "InputDetection::InputDetectionThread : START_WATCHDOG" << std::endl;

			// TODO: Turn the LEDs ON

		}

		while( ( in8(((InputDetection*)arg)->daio_portA_handle) && 0x01) == 0)
		{
			// Stop resetting watchdog
			watchdogFlag = STOP_WATCHDOG;
			// std::cout << "InputDetection::InputDetectionThread : STOP_WATCHDOG" << std::endl;

			// TODO: Turn the LEDs OFF

		}

	}
	return 0;
}
