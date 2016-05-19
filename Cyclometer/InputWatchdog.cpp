/*
 * InputWatchdog.cpp
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#include "InputWatchdog.h"

Signal watchdogFlag = NO_SIGNAL;

InputWatchdog::InputWatchdog(StateContext* state_machine){

	m_state_machine = state_machine;
	watchdogTimer = 3000;

	// Some initializations
	int privity_err;
	
	/* Give this thread root permissions to access the hardware */
	privity_err = ThreadCtl(_NTO_TCTL_IO, NULL);
	
	if (privity_err == -1) {
		std::cout <<"can't get root permissions" << std::endl;
		return;
	}

	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);		// initialize thread attributes structure
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

	// Start the thread
	pthread_create(	&InputWatchdogThreadID, 
					&threadAttr, 
					&InputWatchdog::InputWatchdogThread, 
					this);
}

InputWatchdog::InputWatchdog(){}

InputWatchdog::~InputWatchdog() {

	// Some uninitializations
	pthread_join(InputWatchdogThreadID, NULL);
}


void* InputWatchdog::InputWatchdogThread(void* arg)
{
	((InputWatchdog*)arg)->watchdogTimer = 3000;  // 3 sec timer ==> 0.5ms * 6000

	while( true ) {
		std::cout << "InputWatchdog::InputWatchdogThread" << std::endl;
		
		while( watchdogFlag != START_WATCHDOG &&
				watchdogFlag == STOP_WATCHDOG)
		{
			
		}
		
		while( watchdogFlag == START_WATCHDOG && 
				watchdogFlag != STOP_WATCHDOG )
		{			
			if( ((InputWatchdog*)arg)->watchdogTimer > 0 )
			{
				--((InputWatchdog*)arg)->watchdogTimer;
			}
			else
			{
				((InputWatchdog*)arg)->watchdogTimer = 3000;
			}
		}

	}
	return 0;
}



