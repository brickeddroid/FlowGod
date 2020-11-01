#ifndef PIDHANDLE_HPP
#define PIDHANDLE_HPP

#include "Arduino.h"

#define PID_ACTIVE 1
#define PID_KIFLAG 2

#define PID_OUTMIN 	12544
#define PID_OUTMAX 	65535
#define PID_PWMMIN 	50


struct PidHandle
{
    public:
		PidHandle();
	
		uint8_t output;

		uint8_t state:1;
	
		bool calculate(uint16_t input, uint8_t *params);	
		void initialize(uint16_t input);
		void enableAutoMode(bool enabled, uint16_t input);
	
		//void setParam(int8_t val, uint8_t indx);
		//uint8_t getParam(uint8_t indx);
    
    private:
		uint16_t  lInput;
	
		int32_t outputSum;    
};

#endif
