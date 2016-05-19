/*
 * StateContext.h
 *
 *  Created on: May 3, 2016
 *      Author: mss9627
 */

#ifndef STATECONTEXT_H_
#define STATECONTEXT_H_

using namespace std;

#include "State.h"
#include "Transition.h"
#include "Signals.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <string>

class StateContext
{

protected:
	// Each state represents one orthogonal region
	StateEnum main_state;
	StateEnum display_state;
	StateEnum display_state_internal;
	StateEnum calculation_state;
	StateEnum detection_state;
	StateEnum watchdog_state;

	pthread_t thread;
	bool threadAlive;
	map< int , std::vector< int > > state_table;
	std::vector<State*> states;
	std::vector<Transition*> transitions;

	void createTransitions();
	void createStates();
	void mapTransitionsToStates();
	void performTransition(std::vector<int> transitions, Signal s, StateEnum &state);

public:

	// Threads can access current states but cannot directly modify them
	StateEnum getMainState() {return main_state;}
	StateEnum getDisplayState() {return display_state;}
	StateEnum getDisplayStateInternal() {return display_state_internal;}
	StateEnum getCalculationState() {return calculation_state;}
	StateEnum getDetectionState() {return detection_state;}
	StateEnum getWatchdogState() {return watchdog_state;}

	void acceptSignal(Signal s);
	StateContext();
	~StateContext();
};

#endif /* STATECONTEXT_H_ */
