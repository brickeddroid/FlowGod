#include "helper.hpp"

uint8_t switchBit(uint8_t flag, uint8_t bit, bool sOn)
{
  return (sOn) ? flag | bit : flag & ~bit;
}

bool XOR(bool a, bool b)
{
  return (a+b) % 2;
}

uint32_t convHrsToMill(uint8_t hrs)
{
  return (uint32_t)hrs*60*60*1000;
}

uint32_t convMinToMill(uint8_t min)
{
  return (uint32_t)min*60*1000;
}

uint16_t convertEncU8toU16(uint8_t val)
{
  return (uint16_t)(val>>1) * 10 + (val & 1) * 5;
}
