/*
 * DisplayOperations.h
 *
 *  Created on: May 5, 2016
 *      Author: mss9627
 */

#ifndef DISPLAYOPERATIONS_H_
#define DISPLAYOPERATIONS_H_

enum DisplayMode
{
	KILOMETERS = 0,
	MILES = 1
};

class DisplayOperations
{
protected:
	DisplayMode m_display_mode;
	int m_circumference;
public:
	int getCircumference() { return m_circumference; }
	DisplayMode getDisplayMode() { return m_display_mode; }
};

#endif /* DISPLAYOPERATIONS_H_ */
