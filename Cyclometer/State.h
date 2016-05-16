/*
 * State.h
 *
 *  Created on: May 3, 2016
 *      Author: mss9627
 */

#ifndef STATE_H_
#define STATE_H_

enum StateEnum
{
	FULL_RESET       = 0,
	RUN_CYCLOMETER   = 1,
	SET_UNITS        = 2,
	SET_TIRE_CIRC    = 3,
	CYCLE_TIRE_CIRC  = 4,
	DISPLAY_DATA     = 5,
	DISPLAY_SPEED    = 6,
	DISPLAY_DISTANCE = 7,
	DISPLAY_TIME     = 8,
	MANUAL_OFF       = 9,
	MANUAL_ON        = 10,
	AUTO_OFF         = 11,
	AUTO_ON          = 12,
	INPUT_WAITING    = 13,
	INPUT_POLLING    = 14,
	WATCHDOG_WAITING = 15,
	WATCHDOG_POLLING = 16,

	INVALID_STATE = 17
};

class State
{
protected:
	StateEnum m_state;
public:
	State();
	State(StateEnum state);
	StateEnum getState(){ return m_state; }
	void entryAction();
	void exitAction();
	void doAction();
	~State();
};

#endif /* STATE_H_ */
