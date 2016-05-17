/*
 * InputWatchdog.h
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#ifndef INPUTWATCHDOG_H_
#define INPUTWATCHDOG_H_

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

class InputWatchdog
{

public:

// Attributes
	pthread_t InputWatchdogThreadID;
	unsigned long long watchdogTimer;  // 3 sec timer ==> 0.5ms * 6000

// Methods
	InputWatchdog();	// constructor
	~InputWatchdog();	// destructor

	static void* InputWatchdogThread(void* arg);	// Thread function

};

#endif /* INPUTWATCHDOG_H_ */
