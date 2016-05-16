/*
 * StateContext.cpp
 *
 *  Created on: May 3, 2016
 *      Author: mss9627
 */

#include "StateContext.h"

/* Create transitions and add them to transition vector */
void StateContext::createTransitions()
{
	// FULL_RESET and RUN_CYCLOMETER
	transitions.push_back( new Transition(RUN_CYCLOMETER, NO_SIGNAL) );
	transitions.push_back( new Transition(FULL_RESET, ALL_HELD) );

	 // Null state when orthogonal region is unused
	transitions.push_back( new Transition(INVALID_STATE, NO_SIGNAL) );

/* --------- Display Operations orthogonal region ---------- */

	// SET_UNITS
	transitions.push_back( new Transition(SET_UNITS, MODE) );
	transitions.push_back( new Transition(SET_TIRE_CIRC, SET) );

	// SET_TIRE_CIRC
	transitions.push_back( new Transition(SET_TIRE_CIRC, MODE) );
	transitions.push_back( new Transition(CYCLE_TIRE_CIRC, MODE_HELD) );
	transitions.push_back( new Transition(DISPLAY_DATA, SET) );

	// CYCLE_TIRE_CIRC
	transitions.push_back( new Transition(SET_TIRE_CIRC, MODE_RELEASED) );
	transitions.push_back( new Transition(DISPLAY_DATA, SET) );

	// DISPLAY_DATA
	transitions.push_back( new Transition(DISPLAY_DATA, MODE_START_HELD) );
	transitions.push_back( new Transition(SET_TIRE_CIRC, CHANGE_TIRE_CIRC) );

	// DISPLAY_SPEED
	transitions.push_back( new Transition(DISPLAY_SPEED, SET) ); // Sends CHANGE_TIRE_CIRC
	transitions.push_back( new Transition(DISPLAY_DISTANCE, MODE) );

	// DISPLAY_DISTANCE
	transitions.push_back( new Transition(DISPLAY_DISTANCE, SET) ); // Sends CHANGE_CALC_MODE
	transitions.push_back( new Transition(DISPLAY_TIME, MODE) );

	// DISPLAY_TIME
	transitions.push_back( new Transition(DISPLAY_TIME, SET) ); // Sends CHANGE_CALC_MODE
	transitions.push_back( new Transition(DISPLAY_SPEED, MODE) );

/* --------- Input Calculation orthogonal region ---------- */

	// MANUAL_OFF
	transitions.push_back( new Transition(MANUAL_ON, START) ); // Sends START_POLLING
	transitions.push_back( new Transition(AUTO_ON, CHANGE_CALC_MODE) ); // Sends START_POLLING

	// MANUAL_ON
	transitions.push_back( new Transition(MANUAL_ON, PULSE) );
	transitions.push_back( new Transition(MANUAL_OFF, START) ); // Sends STOP_POLLING
	transitions.push_back( new Transition(AUTO_ON, CHANGE_CALC_MODE) );

	// AUTO_OFF
	transitions.push_back( new Transition(AUTO_ON, PULSE) );
	transitions.push_back( new Transition(MANUAL_OFF, CHANGE_CALC_MODE) ); // Sends STOP_POLLING

	// AUTO_ON
	transitions.push_back( new Transition(AUTO_ON, PULSE) );
	transitions.push_back( new Transition(AUTO_OFF, TIMEOUT) );
	transitions.push_back( new Transition(MANUAL_ON, CHANGE_CALC_MODE) );

/* --------- Input Detection orthogonal region ---------- */

	// INPUT_WAITING
	transitions.push_back( new Transition(INPUT_POLLING, START_POLLING) );

	// INPUT_POLLING
	transitions.push_back( Transition(INPUT_POLLING, NO_SIGNAL) ); // Sends PULSE
	transitions.push_back( new Transition(INPUT_WAITING, STOP_POLLING) );

/* --------- Input Watchdog orthogonal region ---------- */

	// WATCHDOG_WAITING
	transitions.push_back( new Transition(WATCHDOG_POLLING, START_WATCHDOG) );

	// WATCHDOG_POLLING
	transitions.push_back( new Transition(WATCHDOG_WAITING, NO_SIGNAL) ); // Sends TIMEOUT
	transitions.push_back( new Transition(WATCHDOG_WAITING, STOP_WATCHDOG) );
	transitions.push_back( new Transition(WATCHDOG_POLLING, STROKE) );
}

StateContext::StateContext()
{
	threadAlive = true;
	createTransitions();
}

StateContext::~StateContext(){}
