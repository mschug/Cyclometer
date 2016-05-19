/*
 * InputCalculation.cpp
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

#include "InputCalculation.h"

InputCalculation::InputCalculation(StateContext* state_machine, DisplayOperations* display)
{
	m_state_machine = state_machine;
	m_display = display;
	m_last_pulse_time = 0;
	m_current_pulse_time = 0;
	m_trip_time = 0;

	// Stored in kilometers to make calculation easier.
	// Convert to miles by dividing with 1.618.
	m_distance = 0;
	m_average_speed = 0;
	m_current_speed = 0;
}

void InputCalculation::notifyPulse(unsigned long long time)
{
	StateEnum current_state = m_state_machine->getCalculationState();
	if (current_state != MANUAL_OFF && current_state != INVALID_STATE)
	{
		m_last_pulse_time = m_current_pulse_time;
		m_current_pulse_time = time;
		performCalculations();
	}
}

void InputCalculation::performCalculations()
{
	unsigned long long difference = m_current_pulse_time - m_last_pulse_time;

	// v = d/t = circumference/difference with units cm/.5msec
	// Convert from cm/.5msec to km/hr by multiplying by 18
	m_current_speed = (m_display->getCircumference() / difference) * 18;

	// Convert from km to cm by multiplying by 10^-5
	m_distance += (m_display->getCircumference() / 100000);

	// avg(v) = sum(d) / sum(t)
	m_average_speed = (m_distance / m_trip_time) * 18;
}

/* Zero out all values but remain in current mode */
void InputCalculation::resetTripValues()
{
	m_last_pulse_time = 0;
	m_current_pulse_time = 0;
	m_trip_time = 0;
	m_distance = 0;
	m_average_speed = 0;
	m_current_speed = 0;
}

/* Increments the trip time if the cyclometer detects motion.
 * Called once every second based on the global timer. */
void InputCalculation::updateTripTime()
{
	StateEnum state = m_state_machine->getCalculationState();
	if (state == AUTO_ON || state == MANUAL_ON)
	{
		m_trip_time++;
	}
}

/* Returns the current trip time as a single 4 digit number with minutes and seconds.
 * The output value will range between 0 and 9959. */
void InputCalculation::calculateTripTime()
{
	unsigned int seconds = (m_trip_time >= 6000 ? 59 : m_trip_time % 60);
	unsigned int minutes = (m_trip_time >= 6000 ? 99 : m_trip_time / 60);
	m_display->setTime((minutes * 100) + seconds);
}

/* Returns a 4 digit number with the tenths units in the ones place.
 * Can be directly used for output after adding decimal point and a leading zero if necessary. */
void InputCalculation::calculateDistance()
{
	float dist = m_distance;
	if (m_display->getDisplayMode() == MILES) dist /= 1.618;

	unsigned int output;

	if (dist < .1) output = 0;
	else if (dist < 100) output = (unsigned int) (10 * dist);
	else output = 9999;

	m_display->setDistance(output);
}

/* Returns a 6 digit number in the following format:
 * [1 if decmial for current][1 if decimal for average][2 digits of current speed][2 digits of average speed]
 * To output the correct values, read the first two digits and insert decimal points if they are set to 1.
 */
void InputCalculation::calculateSpeed()
{
	unsigned int output = 0;
	bool convert = (m_display->getDisplayMode() == MILES);
	float current_speed = (convert ? (m_current_speed / 1.618) : m_current_speed);
	float average_speed = (convert ? (m_average_speed / 1.618) : m_average_speed);

	if (current_speed >= 100)    output += 9900;
	else if (current_speed > 10) output += ((unsigned int) (current_speed) % 100) * 100;
	else
	{
		output += 100000;
		output += ((unsigned int) (current_speed * 10) % 100) * 100;
	}

	if (average_speed >= 100)    output += 99;
	else if (average_speed > 10) output += ((unsigned int) (average_speed) % 100);
	else
	{
		output += 10000;
		output += ((unsigned int) (average_speed * 10) % 100);
	}

	m_display->setSpeed(output);
}

InputCalculation::~InputCalculation(){}
