/*
 * Signals.h
 *
 *  Created on: May 3, 2016
 *      Author: mss9627
 */

#ifndef SIGNALS_H_
#define SIGNALS_H_

enum Signal
{
	MODE             = 1,
	SET              = 2,
	START            = 3,
	MODE_HELD        = 4,
	MODE_RELEASED    = 5,
	MODE_START_HELD  = 6,
	ALL_HELD         = 7,
	CHANGE_TIRE_CIRC = 8,
	CHANGE_CALC_MODE = 9,
	STOP_POLLING     = 10,
	START_POLLING    = 11,
	PULSE            = 12,
	STOP_WATCHDOG    = 13,
	START_WATCHDOG   = 14,
	STROKE           = 15,
	TIMEOUT          = 16,
	NO_SIGNAL        = 17,

	UNKNOWN_SIGNAL   = -1
};

#endif /* SIGNALS_H_ */
