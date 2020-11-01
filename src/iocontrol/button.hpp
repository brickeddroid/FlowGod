#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Arduino.h"

// ----- Callback function types -----

extern "C" {
  typedef void (*callbackFunction)(void);
}


class Button
{
  public:
    Button(callbackFunction cFunc, /*callbackFunction dcFunc,*/ callbackFunction lpsFunc, uint8_t btnPin=4);
    
    //void begin(callbackFunction cFunc, callbackFunction dcFunc, callbackFunction lpsFunc);
    void update();
    
  private:
    uint8_t btnpin;
    callbackFunction clickFunc;
    //callbackFunction doubleClickFunc;
    callbackFunction longPressStartFunc;
    
    const int debounceTicks; 	// number of ticks for debounce times.
    const int clickTicks; 	// number of ticks that have to pass by before a click is detected
    const int pressTicks; 	// number of ticks that have to pass by before a long button press is detected
    
    uint8_t buttonReleased;
    uint8_t buttonPressed;
    uint8_t state;    
    
    unsigned long startTime; // will be set in state 1
    //unsigned long stopTime; // will be set in state 2
};
 

#endif
