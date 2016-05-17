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

struct sigevent event;
int chid = 0;

int main ( int argc, char *argv[] ) {
	
	int pulse_id = 0 ;

	// Create a channel to receive timer events on
	chid = ChannelCreate( 0 );
	assert ( chid != -1 );			// if returns a -1 for failure we stop with error

	// Set up the timer and timer event
	event.sigev_notify = SIGEV_PULSE;		// most basic message we can send -- just a pulse number
	event.sigev_coid = ConnectAttach ( 0, 0, chid, 0, 0 );  // Get ID that allows me to communicate on the channel
	assert ( event.sigev_coid != -1 );		// stop with error if cannot attach to channel
	event.sigev_priority = getprio(0);
	event.sigev_code = 1023;				// arbitrary number assigned to this pulse
	event.sigev_value.sival_ptr = (void*)pulse_id;
	


	// Create all the threads and then create the global timer thread
	// Global timer thread signals other threads to start

	InputDetection inputDetection;
	InputWatchdog inputWatchdog;
	GlobalTimer globalTimer;




	return EXIT_SUCCESS;
}
