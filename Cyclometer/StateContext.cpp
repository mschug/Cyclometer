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

	 // Exit state when orthogonal region must exit
	transitions.push_back( new Transition(INVALID_STATE, REGION_EXIT_SIGNAL) );

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

	// Exit states below when DISPLAY_DATA exits
	transitions.push_back( new Transition(INVALID_STATE, CHANGE_TIRE_CIRC) );

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

/* Maps states to their transitions
 * The order of transitions is determined by createTransitions()
 * If the order changes, the mapping will be incorrect
 */
void StateContext::mapTransitionsToStates()
{
	// FULL_RESET and RUN_CYCLOMETER
	std::vector< int > reset_state(1);
	reset_state[0] = 0;
	std::vector< int > run_state(1);
	run_state[0] = 1;
	state_table[0] = reset_state;
	state_table[1] = run_state;

	// All states below include a transition to an invalid state once full reset occurs

/* --------- Display Operations orthogonal region ---------- */

	const int RESET_TRANSITON = 2;

	// SET_UNITS
	std::vector< int > set_unit_state(3);
	set_unit_state[0] = RESET_TRANSITION;
	set_unit_state[1] = 3;
	set_unit_state[2] = 4;
	state_table[2] = set_unit_state;

	// SET_TIRE_CIRC
	std::vector< int > set_circ_state(4);
	set_circ_state[0] = RESET_TRANSITION;
	set_circ_state[1] = 5;
	set_circ_state[2] = 6;
	set_circ_state[3] = 7;
	state_table[3] = set_circ_state;

	// CYCLE_TIRE_CIRC
	std::vector< int > cycle_circ_state(3);
	cycle_circ_state[0] = RESET_TRANSITION;
	cycle_circ_state[1] = 8;
	cycle_circ_state[2] = 9;
	state_table[4] = cycle_circ_state;

	// DISPLAY_DATA
	std::vector< int > display_data_state(3);
	display_data_state[0] = RESET_TRANSITION;
	display_data_state[1] = 10;
	display_data_state[2] = 11;
	state_table[5] = display_data_state;

	// The following states will also exit to an invalid state if DISPLAY_DATA exits
	const int DISPLAY_TRANSITION = 12;

	// DISPLAY_SPEED
	std::vector< int > display_speed_state(4);
	display_speed_state[0] = RESET_TRANSITION;
	display_speed_state[1] = DISPLAY_TRANSITION;
	display_speed_state[2] = 13;
	display_speed_state[3] = 14;
	state_table[6] = display_speed_state;

	// DISPLAY_DISTANCE
	std::vector< int > display_distance_state(4);
	display_distance_state[0] = RESET_TRANSITION;
	display_distance_state[1] = DISPLAY_TRANSITION;
	display_distance_state[2] = 15;
	display_distance_state[3] = 16;
	state_table[7] = display_distance_state;

	// DISPLAY_TIME
	std::vector< int > display_time_state(4);
	display_time_state[0] = RESET_TRANSITION;
	display_time_state[1] = DISPLAY_TRANSITION;
	display_time_state[2] = 17;
	display_time_state[3] = 18;
	state_table[8] = display_time_state;

/* --------- Input Calculation orthogonal region ---------- */

	// MANUAL_OFF
	std::vector< int > manual_off_state(3);
	manual_off_state[0] = RESET_TRANSITION;
	manual_off_state[1] = 19;
	manual_off_state[2] = 20;
	state_table[9] = manual_off_state;

	// MANUAL_ON
	std::vector< int > manual_on_state(4);
	manual_on_state[0] = RESET_TRANSITION;
	manual_on_state[1] = 21;
	manual_on_state[2] = 22;
	manual_on_state[3] = 23;
	state_table[10] = manual_on_state;

	// AUTO_OFF
	std::vector< int > auto_off_state(3);
	auto_off_state[0] = RESET_TRANSITION;
	auto_off_state[1] = 24;
	auto_off_state[2] = 25;
	state_table[11] = auto_off_state;

	// AUTO_ON
	std::vector< int > auto_on_state(4);
	auto_on_state[0] = RESET_TRANSITION;
	auto_on_state[1] = 26;
	auto_on_state[2] = 27;
	auto_on_state[3] = 28;
	state_table[12] = auto_on_state;

/* --------- Input Detection orthogonal region ---------- */

	// INPUT_WAITING
	std::vector< int > input_waiting_state(2);
	input_waiting_state[0] = RESET_TRANSITION;
	input_waiting_state[1] = 29;
	state_table[13] = input_waiting_state;

	// INPUT_POLLING
	std::vector< int > input_polling_state(3);
	input_polling_state[0] = RESET_TRANSITION;
	input_polling_state[1] = 30;
	input_polling_state[2] = 31;
	state_table[14] = input_polling_state;

/* --------- Input Watchdog orthogonal region ---------- */

	// WATCHDOG_WAITING
	std::vector< int > watchdog_waiting_state(2);
	watchdog_waiting_state[0] = RESET_TRANSITION;
	watchdog_waiting_state[1] = 32;
	state_table[15] = watchdog_waiting_state;

	// WATCHDOG_POLLING
	std::vector< int > watchdog_polling_state(4);
	watchdog_polling_state[0] = RESET_TRANSITION;
	watchdog_polling_state[1] = 33;
	watchdog_polling_state[2] = 34;
	watchdog_polling_state[3] = 35;
	state_table[16] = watchdog_polling_state;
}

StateContext::StateContext()
{
	threadAlive = true;
	createTransitions();
	mapTransitionsToStates();
}

StateContext::~StateContext(){}
