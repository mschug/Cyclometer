/*
 * GlobalTimer.cpp
 *
 *  Created on: May 5, 2016
 *      Author: ssj3000/mss9627
 */

#include "GlobalTimer.h"

unsigned long long gblCounter = 0;


GlobalTimer::GlobalTimer() {

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
	pthread_create(	&GlobalTimerThreadID,
					&threadAttr,
					&GlobalTimer::GlobalTimerThread,
					this);
}

GlobalTimer::~GlobalTimer() {

	// Some uninitializations
	pthread_join(GlobalTimerThreadID, NULL);
}

void* GlobalTimer::GlobalTimerThread(void* arg)
{
	// int pid;
	// int chid;
	// int pulse_id = 0 ;
	timer_t timer_id;
	// struct sigevent event;
	struct itimerspec timer;
	struct _clockperiod clkper;
	// struct _pulse pulse;
	// int count = 0 ;
	struct sched_param param;
	int ret;

	// Set our priority to the maximum,
	// so we wont get disrupted by anything other than interrupts
	param.sched_priority = sched_get_priority_max( SCHED_RR );
	ret = sched_setscheduler( 0, SCHED_RR, &param);
	assert ( ret != -1 );	// if returns a -1 for failure we stop with error

	// Setup clock
	clkper.nsec = 500000;
	clkper.fract = 0;
	ClockPeriod ( CLOCK_REALTIME, &clkper, NULL, 0 ); // 1ms

// ----------------------------------------------------
// The channel ID and Event struct is created in main.c
// and are used by all threads for communication
// ----------------------------------------------------
	// // Create a channel to receive timer events on
	// chid = ChannelCreate( 0 );
	// assert ( chid != -1 );			// if returns a -1 for failure we stop with error

	// // Set up the timer and timer event
	// event.sigev_notify = SIGEV_PULSE;
	// event.sigev_coid = ConnectAttach ( ND_LOCAL_NODE, 0, chid, 0, 0 );
	// assert ( event.sigev_coid != -1 );		// stop with error if cannot attach to channel
	// event.sigev_priority = getprio(0);
	// event.sigev_code = 1023;				// arbitrary number assigned to this pulse
	// event.sigev_value.sival_ptr = (void*)pulse_id;
// ----------------------------------------------------


	// Now create the timer and get back the timer_id value for the timer we created
	// CLOCK_REALTIME available in all POSIX systems
	if ( timer_create( CLOCK_REALTIME, &event, &timer_id ) == -1 )
	{
		perror ( "can�t create timer" );
		exit( EXIT_FAILURE );
	}

	// Change the timer configuration to set its period interval
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_nsec = 1000000;		// interrupt at 1 ms.
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 1000000;		// keep interrupting every 1 ms.

	// Start the timer
	if ( timer_settime( timer_id, 0, &timer, NULL ) == -1 )
	{
		perror("Can\'t start timer.\n");
		exit( EXIT_FAILURE );
	}

	// send a pulse
	// MsgSendPulse ( int coid, int priority, int code, int value );


	gblCounter = 0;

	// start the global counter
	for( ; ; )
	{
		gblCounter++;
		std::cout << "GlobalTimer::GlobalTimerThread: " << gblCounter << std::endl;
	}

	return 0;
}
