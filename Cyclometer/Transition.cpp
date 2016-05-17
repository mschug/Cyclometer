/*
 * Transition.cpp
 *
 *  Created on: May 3, 2016
 *      Author: mss9627
 */

#include "Transition.h"

Transition::Transition()
{
	m_signal = UNKNOWN_SIGNAL;
	m_next_state = INVALID_STATE;
}

Transition::Transition(StateEnum next_state, Signal signal)
{
	m_next_state = next_state;
	m_signal = signal;
}

Signal Transition::getSignal()
{
	return m_signal;
}

StateEnum Transition::getNextState()
{
	return m_next_state;
}
