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
	KILOMETERS = 0,
	MILES = 1
};

enum SevenSegment
{
	// Lit segments are represented as 0 bits
	// Output appears in the order abcdefgh
	BLANK = 0xFF,
	ZERO = 0x03,
	ONE = 0x9F,
	TWO = 0x25,
	THREE = 0x0D,
	FOUR = 0x99,
	FIVE = 0x49,
	SIX = 0x41,
	SEVEN = 0x1F,
	EIGHT = 0x01,
	NINE = 0x09,

	// Decimal is used such as BLANK & DECIMAL
	DECIMAL = 0xFE,

	// Displays used as (in8 val | SELECT_DISPLAY) & FIRST_DISPLAY
	SELECT_DISPLAY = 0x0F,
	FIRST_DISPLAY = 0xF7,
	SECOND_DISPLAY = 0xFB,
	THIRD_DISPLAY = 0xFD,
	FOURTH_DISPLAY = 0xFE
};

class DisplayOperations
{
protected:
	DisplayMode m_display_mode;
	int m_circumference;
	StateContext* m_state_machine;
public:
	int getCircumference() { return m_circumference; }
	DisplayMode getDisplayMode() { return m_display_mode; }

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
