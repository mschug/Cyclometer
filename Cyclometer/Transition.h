/*
 * Transition.h
 *
 *  Created on: May 3, 2016
 *      Author: mss9627
 */

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include "Signals.h"
#include "State.h"

class Transition
{
protected:
	Signal m_signal;
	StateEnum m_next_state;
public:
	Signal getSignal();
	StateEnum getNextState();

	Transition();
	Transition(StateEnum next_state, Signal signal);
};

#endif /* TRANSITION_H_ */
