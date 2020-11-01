#ifndef REMOTECONTROL_HPP
#define REMOTECONTROL_HPP

#include "Arduino.h"

class RemoteControl
{
  public:
    static uint8_t const * sysAddrPtr;
    
    static void switchOff(uint8_t address);
    static void switchOn(uint8_t address);
    
    static void begin(uint8_t const * const _sysAddrPtr);
  
  private:
    RemoteControl(){} 
    
    static int16_t pulseLength;
    static uint8_t nRepeat;
    static void    switchRcDevc(uint8_t address, bool bOn);
    static void    transmit(int nHighPulses, int nLowPulses);
    static void    sendT0();
    static void    sendT1();
    
};

#endif
