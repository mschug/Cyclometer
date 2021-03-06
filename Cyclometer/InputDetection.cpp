/*
 * InputDetection.cpp
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#include "InputDetection.h"


InputDetection::InputDetection(StateContext* state_machine, DisplayOperations* display, InputCalculation* calculation){

	m_state_machine = state_machine;
	m_calculation = calculation;
	m_display = display;
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
	out8(daio_ctrl_handle, 0x01 | val); // DIRCL

	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);		// initialize thread attributes structure
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

	// Start the thread
	pthread_create(	&InputDetectionThreadID, 
					&threadAttr, 
					&InputDetection::InputDetectionThread, 
					this);
}

InputDetection::InputDetection(){}
InputDetection::~InputDetection() {

	// Some uninitializations
	pthread_join(InputDetectionThreadID, NULL);
}


void* InputDetection::InputDetectionThread(void* arg)
{
	std::cout << "InputDetection::InputDetectionThread" << std::endl;

    // Give this thread root permissions to access the hardware
    int privity_err = ThreadCtl(_NTO_TCTL_IO, NULL);
    if (privity_err == -1) {
        std::cout <<"can't get root permissions" << std::endl;
        return NULL;
    }

	//bool inputPulseDetected = false;
	watchdogFlag = UNKNOWN_SIGNAL;
	bool pulseFlag = false;

	while(true)
	{
		// check only the PIN connected to pulse generator
		while( ( in8(((InputDetection*)arg)->daio_portC_handle) & 0x08))
		{
			if (!pulseFlag)
			{
				((InputDetection*)arg)->m_state_machine->acceptSignal(PULSE);
				((InputDetection*)arg)->m_calculation->notifyPulse();
				pulseFlag = true;
			}
			// Reset watchdog
			watchdogFlag = START_WATCHDOG;
			// std::cout << "InputDetection::InputDetectionThread : START_WATCHDOG" << std::endl;

			// TODO: Turn the LEDs ON

		}

		pulseFlag = false;

		while( ( in8(((InputDetection*)arg)->daio_portC_handle) & 0x08))
		{
			// Stop resetting watchdog
			watchdogFlag = STOP_WATCHDOG;
			// std::cout << "InputDetection::InputDetectionThread : STOP_WATCHDOG" << std::endl;

			// TODO: Turn the LEDs OFF

		}

	}
	return 0;
}
