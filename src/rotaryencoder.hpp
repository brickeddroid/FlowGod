#ifndef ROTARYENCODER_HPP
#define ROTARYENCODER_HPP

#include "Arduino.h"

#define RE_ENC_FLAGA 	0b00000001
#define RE_ENC_FLAGB 	0b00000010

#define RE_INTR_FLAGA  	0b00000100
#define RE_INTR_FLAGB	0b00001000

#define RE_INTR_A		0b00001101
#define RE_INTR_B		0b00001110

class RotaryEncoder
{
  public:
    static void begin();
    static bool hasChanged();
    
  private:
    RotaryEncoder();
    
    static void interruptPinA();
    static void interruptPinB();
    
    //static volatile uint8_t reading;
    static volatile uint8_t flag;
    
};

#endif
