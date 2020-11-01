#ifndef HELPER_HPP
#define HELPER_HPP

#include "Arduino.h"

uint8_t switchBit(uint8_t flag, uint8_t bit, bool sOn);
bool XOR(bool a, bool b);

uint32_t convMinToMill(uint8_t min);
uint32_t convHrsToMill(uint8_t hrs);
uint16_t convertEncU8toU16(uint8_t val);


#endif
