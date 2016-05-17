/*
 * GlobalTimer.h
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#ifndef GLOBALTIMER_H_
#define GLOBALTIMER_H_

#include <pthread.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <unistd.h>			// for sleep()
#include <stdint.h>			// for uintptr_t
#include <hw/inout.h>		// for in*() and out*() functions
#include <sys/mman.h>		// for mmap_device_io()
#include <inttypes.h>		// for ClockCycles() return value
#include <assert.h>

#include "Signals.h"

class GlobalTimer
{

public:

	// Thread ID
	pthread_t GlobalTimerThreadID;


	GlobalTimer();	// constructor
	~GlobalTimer();	// destructor

	static void* GlobalTimerThread(void* arg);	// Thread function

};

#endif /* GLOBALTIMER_H_ */
