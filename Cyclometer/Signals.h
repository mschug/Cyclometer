/*
 * Signals.h
 *
 *  Created on: May 3, 2016
 *      Author: ssj3000/mss9627
 */

#ifndef SIGNALS_H_
#define SIGNALS_H_

enum Signal
{
	MODE               = 0,
	SET                = 1,
	START              = 2,
	STOP               = 3,
	MODE_HELD          = 4,
	MODE_RELEASED      = 5,
	MODE_START_HELD    = 6,
	ALL_HELD           = 7,
	CHANGE_TIRE_CIRC   = 8,
	CHANGE_CALC_MODE   = 9,
	STOP_POLLING       = 10,
	START_POLLING      = 11,
	PULSE              = 12,
	STOP_WATCHDOG      = 13,
	START_WATCHDOG     = 14,
	STROKE             = 15,
	TIMEOUT            = 16,
	NO_SIGNAL          = 17,
	REGION_EXIT_SIGNAL = 18,

	UNKNOWN_SIGNAL = 19
};

// For communicating signals among threads
extern Signal last_signal;
extern Signal curr_signal;

// For global timer
extern unsigned long long gblCounter;

// Watchdog flag
extern Signal watchdogFlag;

// Port length and memory mapped register addresses
#define PORT_LENGTH                 1
#define BASE_ADDRESS                0x280
#define DAIO_PORTA_ADDRESS          0x08
#define DAIO_PORTB_ADDRESS          0x09
#define DAIO_PORTC_ADDRESS          0x0A
#define DAIO_CONTROLREG_ADDRESS     0x0B


// Timing Macros
#define TIMERTICK_NS 1000000		// 1 msec
#define WATCHDOG_TIMEOUT 3000		// 3000 msec
#define PBPRESS_TIMEOUT 2000	 	// 2000 msec
#define PBSIMULPRESS_TIMEOUT 200 	// 200 msec



#endif /* SIGNALS_H_ */
