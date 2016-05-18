/*
 * InputDetection.h
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#ifndef INPUTDETECTION_H_
#define INPUTDETECTION_H_

#include <pthread.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <unistd.h>			// for sleep()
#include <stdint.h>			// for uintptr_t
#include <hw/inout.h>		// for in*() and out*() functions
#include <sys/mman.h>		// for mmap_device_io()
#include <inttypes.h>		// for ClockCycles() return value

#include "Signals.h"

class InputDetection
{

public:

	// Thread ID
	pthread_t InputDetectionThreadID;

	// Handle variables for DAIO Port C for input pulse detection
	uintptr_t daio_ctrl_handle;
	uintptr_t daio_portC_handle;

	InputDetection();	// constructor
	~InputDetection();	// destructor


	static void* InputDetectionThread(void* arg);	// Thread function

};

#endif /* INPUTDETECTION_H_ */
