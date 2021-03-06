/*
 * Cyclometer.cpp
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#include <cstdlib>
#include <iostream>
#include <assert.h>
#include <sys/neutrino.h>

#include "Cyclometer.h"

Signal last_signal = NO_SIGNAL;
Signal curr_signal = NO_SIGNAL;

int main ( int argc, char *argv[] )
{
	
	// Create all the threads and then create the global timer thread
	// Global timer thread signals other threads to start

	GlobalTimer globalTimer;
	StateContext stateMachine;
	InputWatchdog inputWatchdog(&stateMachine);
	DisplayOperations displayOperations(&stateMachine);
	InputCalculation inputCalculation(&stateMachine, &displayOperations);
	InputDetection inputDetection(&stateMachine, &displayOperations, &inputCalculation);
	PushButtonDetection pushbuttonDetection(&stateMachine, &displayOperations, &inputCalculation);

	return EXIT_SUCCESS;
}
