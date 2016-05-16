/*
 * InputCalculation.h
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

#ifndef INPUTCALCULATION_H_
#define INPUTCALCULATION_H_

class InputCalculation
{
protected:
	unsigned long long m_last_pulse_time;
	int m_pulses;
	int m_trip_time;

	// Stored in kilometers to make calculation easier.
	// Convert to miles by multiplying with 1.618.
	float m_distance;
	float m_average_speed;
	float m_current_speed;

	void performCalculations();
public:
	// Return 4 digit values to be used by DisplayOperations.
	unsigned int getTripTime();
	unsigned int getDistance();
	unsigned int getSpeeds();

	void notifyPulse(unsigned long long time);
	void resetTripValues();
	InputCalculation();
	~InputCalculation();
};
#endif /* INPUTCALCULATION_H_ */
