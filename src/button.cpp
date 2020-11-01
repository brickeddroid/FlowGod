#include "button.hpp"

Button::Button(callbackFunction cFunc, /*callbackFunction dcFunc,*/ callbackFunction lpsFunc, uint8_t pin)
  : btnpin(pin), debounceTicks(40), clickTicks(500),
    pressTicks(800), buttonReleased(HIGH), buttonPressed(LOW),
    clickFunc(cFunc), /*doubleClickFunc(NULL),*/ longPressStartFunc(lpsFunc)
{
  state = 0; // starting with state 0: waiting for button to be pressed

  // use the given pin as input and activate internal PULLUP resistor.
  pinMode( btnpin, INPUT_PULLUP );
} // Button



//void Button::begin(callbackFunction cFunc, callbackFunction dcFunc, callbackFunction lpsFunc)
//{
//  clickFunc = cFunc;
//  doubleClickFunc = dcFunc;
//  longPressStartFunc = lpsFunc;
//}

void Button::update()
{
  // Detect the input information 
  uint8_t buttonLevel = digitalRead(btnpin); // current button signal.
  unsigned long now = millis(); // current (relative) time in msecs.
  // Implementation of the state machine
  if (state == 0) { // waiting for menu pin being pressed.
    if (buttonLevel == buttonPressed) {
      state = 1; // step to state 1
      startTime = now; // remember starting time
    } // if

  } else if (state == 1) { // waiting for menu pin being released.

    if ((buttonLevel == buttonReleased) && ((unsigned long)(now - startTime) < debounceTicks)) {
      // button was released to quickly so I assume some debouncing.
	  // go back to state 0 without calling a function.
      state = 0;

    } else if (buttonLevel == buttonReleased) {
      state = 2; // step to state 2
      //stopTime = now; // remember stopping time

    } else if ((buttonLevel == buttonPressed) && ((unsigned long)(now - startTime) > pressTicks)) {
	//isLongPressed = true;  // Keep track of long press state
	longPressStartFunc();
	state = 3; // step to state 6
      
    } else {
      // wait. Stay in this state.
    } // if

  } else if (state == 2) { // waiting for menu pin being pressed the second time or timeout.
    if (/*doubleClickFunc == NULL ||*/ (unsigned long)(now - startTime) > clickTicks) {
      // this was only a single short click
      clickFunc();
      state = 0; // restart.

    } /*else if ((buttonLevel == buttonPressed) && ((unsigned long)(now - stopTime) > debounceTicks)) {
      state = 3; // step to state 3
      startTime = now; // remember starting time
    } // if

  } else if (state == 3) { // waiting for menu pin being released finally.
    // Stay here for at least _debounceTicks because else we might end up in state 1 if the
    // button bounces for too long.
    if (buttonLevel == buttonReleased && ((unsigned long)(now - startTime) > debounceTicks)) {
      // this was a 2 click sequence.
      //doubleClickFunc();
      state = 0; // restart.
    } // if */

  } else if (state == 3) { // waiting for menu pin being release after long press.
    if (buttonLevel == buttonReleased) {
	//isLongPressed = false;  // Keep track of long press state
        state = 0; // restart.
    }  

  } // if  
} // OneButton.tick()
