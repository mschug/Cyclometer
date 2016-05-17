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
	MODE_HELD          = 3,
	MODE_RELEASED      = 4,
	MODE_START_HELD    = 5,
	ALL_HELD           = 6,
	CHANGE_TIRE_CIRC   = 7,
	CHANGE_CALC_MODE   = 8,
	STOP_POLLING       = 9,
	START_POLLING      = 10,
	PULSE              = 11,
	STOP_WATCHDOG      = 12,
	START_WATCHDOG     = 13,
	STROKE             = 14,
	TIMEOUT            = 15,
	NO_SIGNAL          = 16,
	REGION_EXIT_SIGNAL = 17,

	UNKNOWN_SIGNAL = 18
};

//extern Signal signal;
extern unsigned long long gblCounter;
extern struct sigevent event;
extern int chid;


// Watchdog flag
extern Signal watchdogFlag;

// Port length and memory mapped register addresses
#define PORT_LENGTH                 1
#define BASE_ADDRESS                0x280
#define DAIO_PORTA_ADDRESS          0x08  // For voltage data
#define DAIO_PORTB_ADDRESS          0x09  // For SyncClock
#define DAIO_PORTC_ADDRESS          0x0A  // For parity bit
#define DAIO_CONTROLREG_ADDRESS     0x0B


#endif /* SIGNALS_H_ */
