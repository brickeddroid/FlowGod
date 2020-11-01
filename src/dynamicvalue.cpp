/*generated file dynamicvalue.cpp*/
#include "dynamicvalue.hpp"

volatile uint8_t DynamicValue::state 	 = 0;
volatile uint8_t DynamicValue::value 	 = 0;
volatile uint8_t DynamicValue::min       = 0;
volatile uint8_t DynamicValue::max 		 = 255;
volatile uint8_t DynamicValue::stepwidth = 1;


void DynamicValue::set(uint8_t _value, uint8_t _min, uint8_t _max, uint8_t _stepwidth)
{
	noInterrupts();
	value 		= _value;
	min   		= _min;
	max 		= _max;
	stepwidth 	= _stepwidth;
	state 		= 0;
	interrupts();
}

//EOF DYNAMICVALUE_CPP
