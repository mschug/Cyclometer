/*
 * DisplayOperations.h
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

#ifndef DISPLAYOPERATIONS_H_
#define DISPLAYOPERATIONS_H_

#include <pthread.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <unistd.h>			// for sleep()
#include <stdint.h>			// for uintptr_t
#include <hw/inout.h>		// for in*() and out*() functions
#include <sys/mman.h>		// for mmap_device_io()
#include <inttypes.h>		// for ClockCycles() return value

#include "Signals.h"
#include "State.h"
#include "StateContext.h"

enum DisplayMode
{
	KILOMETERS = 1,
	MILES = 2
};

enum SevenSegment
{
	// Lit segments are represented as 0 bits
	// Output appears in the order hgfedcba
	BLANK = 0xFF,
	ZERO = 0xC0,
	ONE = 0xF9,
	TWO = 0xA4,
	THREE = 0xB0,
	FOUR = 0x99,
	FIVE = 0x92,
	SIX = 0x82,
	SEVEN = 0xF8,
	EIGHT = 0x80,
	NINE = 0x90,

	// Decimal is used such as BLANK & DECIMAL
	DECIMAL = 0x7F,

	// Dashes are displayed during a full system reset (or error state)
	DASH = 0xBF,

	// Displays used as (in8 val | SELECT_DISPLAY) & FIRST_DISPLAY
	SELECT_DISPLAY = 0x0F,
	FIRST_DISPLAY = 0xFE,
	SECOND_DISPLAY = 0xFD,
	THIRD_DISPLAY = 0xFB,
	FOURTH_DISPLAY = 0xF7
};

class DisplayOperations
{
protected:
	void numberToOutput(unsigned int* output, unsigned int num);
	void sendOutput(unsigned int* output);

	DisplayMode m_display_mode;
	unsigned int m_circumference;
	StateContext* m_state_machine;
	unsigned int m_speed;
	unsigned int m_distance;
	unsigned int m_time;

public:
	int getCircumference()       { return m_circumference; }
	DisplayMode getDisplayMode() { return m_display_mode; }
	void setSpeed(unsigned int speed)       { m_speed = speed; }
	void setDistance(unsigned int distance) { m_distance = distance; }
	void setTime(unsigned int trip_time)    { m_time = trip_time; }

	// Thread initialization
	pthread_t DisplayOperationsThreadID;
	static void* DisplayOperationsThread(void* arg);
	bool threadAlive;

	// Handle variables for DAIO Port A and B for output
	uintptr_t daio_ctrl_handle;
	uintptr_t daio_portA_handle;
	uintptr_t daio_portB_handle;

	DisplayOperations();
	DisplayOperations(StateContext* state_machine);
	~DisplayOperations();
};

#endif /* DISPLAYOPERATIONS_H_ */
