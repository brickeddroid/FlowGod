#include "src/board.hpp"
#include <avr/wdt.h>

//#define DEBUG_WDT

void setup() {
  //Serial.begin(9600);
  
  Board::begin();
  wdt_enable(WDTO_2S);
}

void loop() {
  Board::update();
  wdt_reset();
#ifdef DEBUG_WDT
  while(true){
  }
#endif
  
}
