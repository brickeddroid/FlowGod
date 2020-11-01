#ifndef MEMORYHANDLE_HPP
#define MEMORYHANDLE_HPP

#include "Arduino.h"

struct MemoryHandle {
	static uint8_t slot;
	static uint8_t prevslot;
	
	static void begin();
	static void initialize();
	static void update();
};

#endif
