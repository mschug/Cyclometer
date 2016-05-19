/*
 * PushButtonDetection.h
 *
 *  Created on: May 17, 2016
 *      Author: ssj3000
 */

#ifndef PUSHBUTTONDETECTION_H_
#define PUSHBUTTONDETECTION_H_

#include <pthread.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <unistd.h>			// for sleep()
#include <stdint.h>			// for uintptr_t
#include <hw/inout.h>		// for in*() and out*() functions
#include <sys/mman.h>		// for mmap_device_io()
#include <inttypes.h>		// for ClockCycles() return value

#include <string>
#include "Signals.h"
#include "DisplayOperations.h"
#include "InputCalculation.h"
#include "StateContext.h"

class PushButtonDetection
{
protected:
	StateContext* m_state_machine;
	DisplayOperations* m_display;
	InputCalculation* m_calculation;

public:

	// Thread ID
	pthread_t PushButtonDetectionThreadID;

	Signal lastStartStop;

	// Handle variables for DAIO Port C for input pulse detection
	uintptr_t daio_ctrl_handle;
	uintptr_t daio_portC_handle;

	PushButtonDetection(StateContext* state_machine, DisplayOperations* display, InputCalculation* calculation);
	PushButtonDetection();	// constructor
	~PushButtonDetection();	// destructor


	static void* PushButtonDetectionThread(void* arg);	// Thread function

};


#endif /* PUSHBUTTONDETECTION_H_ */
