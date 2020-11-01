#ifndef TIMER_HPP
#define TIMER_HPP

#include "Arduino.h"

struct Timer
{
	public:	
		void start();
		void stop();
		bool alarm(unsigned long interval);
  
	private:
		bool isActive;
		unsigned long beginT;
};

#endif
