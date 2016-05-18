/*
 * InputCalculation.h
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

#ifndef INPUTCALCULATION_H_
#define INPUTCALCULATION_H_

#include "DisplayOperations.h"
#include "StateContext.h"

class InputCalculation
{
protected:
	unsigned long long m_last_pulse_time, m_current_pulse_time;
	unsigned int m_trip_time;

	// Stored in kilometers to make calculation easier.
	// Convert to miles by dividing with 1.618.
	float m_distance;
	float m_average_speed, m_current_speed;

	DisplayOperations* m_display;
	StateContext* m_state_machine;

	void performCalculations();
	// Calculates 4 digit output values and sends them to DisplayOperations.
	void calculateTripTime();
	void calculateDistance();
	void calculateSpeed();

public:
	void updateTripTime();
	void startTrip();
	void stopTrip();
	void notifyPulse(unsigned long long time);
	void resetTripValues();
	InputCalculation(StateContext* state_machine, DisplayOperations* display);
	~InputCalculation();
};
#endif /* INPUTCALCULATION_H_ */
