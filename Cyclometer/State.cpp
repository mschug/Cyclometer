/*
 * State.cpp
 *
 *  Created on: May 3, 2016
 *      Author: mss9627
 */

#include "State.h"

State::State()
{
	m_state = INVALID_STATE;
}

State::State(StateEnum state)
{
	m_state = state;
}

void State::entryAction(){}
void State::exitAction(){}
void State::doAction(){}
