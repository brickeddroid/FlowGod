#ifndef DYNAMICVALUE_HPP
#define DYNAMICVALUE_HPP

#include "Arduino.h"

#define DV_CHANGED	1

struct DynamicValue
{
	static volatile uint8_t state;
	static volatile uint8_t value;
	static volatile uint8_t min;
	static volatile uint8_t max;
	static volatile uint8_t stepwidth;
	
	static void set(uint8_t value, uint8_t min, uint8_t max, uint8_t stepwidth = 1);
};

#endif
