/*
 * InputCalculation.cpp
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

#include "InputCalculation.h"

InputCalculation::InputCalculation(DisplayOperations* display)
{
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
	m_last_pulse_time = m_current_pulse_time;
	m_current_pulse_time = time;
	performCalculations();
}

void InputCalculation::performCalculations()
{
	unsigned long long difference = m_current_pulse_time - m_last_pulse_time;
	m_trip_time += difference;

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

unsigned int InputCalculation::getTripTime()
{
	// TODO: Initialize timer thread
}

/* Returns a 4 digit number with the tenths units in the ones place.
 * Can be directly used for output after adding decimal point and a leading zero if necessary. */
unsigned int InputCalculation::getDistance()
{
	float dist = m_distance;
	if (m_display->getDisplayMode() == MILES) dist /= 1.618;

	if (dist < .1) return 0;
	else if (dist < 100) return (unsigned int) (10 * dist);
	else return 9999;
}

/* Returns a 6 digit number in the following format:
 * [1 if decmial for current][1 if decimal for average][2 digits of current speed][2 digits of average speed]
 * To output the correct values, read the first two digits and insert decimal points if they are set to 1.
 */
unsigned int InputCalculation::getSpeeds()
{
	unsigned int output = 0;

	if (m_current_speed >= 100)    output += 9900;
	else if (m_current_speed > 10) output += ((unsigned int) (m_current_speed) % 100) * 100;
	else
	{
		output += 100000;
		output += ((unsigned int) (m_current_speed * 10) % 100) * 100;
	}

	if (m_average_speed >= 100)    output += 99;
	else if (m_average_speed > 10) output += ((unsigned int) (m_average_speed) % 100);
	else
	{
		output += 10000;
		output += ((unsigned int) (m_average_speed * 10) % 100);
	}

	return output;
}

InputCalculation::~InputCalculation(){}
