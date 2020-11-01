#ifndef STRING_HPP
#define STRING_HPP


//#ifndef DYNAMICSELECTABLEVALUE_HPP
//#define DYNAMICSELECTABLEVALUE_HPP

#include "Arduino.h"

#include <U8x8lib.h>

//#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//#include "dynamicselectablevalue.hpp"

/*
  Universal 8bit Graphics Library (https://github.com/olikraus/ u8x8/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/
//PrintableString metaFlags
#define PS_CLICKONLY	1
#define PS_BIG		2
#define PS_INVERTED	4
#define PS_SELECTED	8
#define PS_SEL_LOCK	16
#define PS_BLINK	32
#define PS_MARKED	64

//#define DISPLAY_MENU_AUTO 	0
//#define DISPLAY_MENU_MANUAL 	1
//#define DISPLAY_MENU_INSPAGE 	2
//#define DISPLAY_MENU_INSVALUE	3

class IPage;

//Words Library in Progmem
const char standbyString[]  	PROGMEM = "STANDBY";
const char switchString[]  	PROGMEM = "SWTIME";
const char pageString[]  	PROGMEM = "PAGES";
//const char setPtString[]  	PROGMEM = "SETPOINT";
const char minString[]    	PROGMEM = "MIN";
const char maxString[]    	PROGMEM = "MAX";
const char dispString[]     PROGMEM = "DISPL.";
const char addrString[]    	PROGMEM = "ADDRESS";
const char sysString[]    	PROGMEM = "SYSTEM";
const char activeString[] 	PROGMEM = "ACTIVE";
const char deactString[]  	PROGMEM = "DEACT.";
const char onString[]     	PROGMEM = "ON ";
const char offString[]    	PROGMEM = "OFF";
const char sensString[]   	PROGMEM = "SENS.";
const char tempString[]   	PROGMEM = "TEMP";
const char humiString[]   	PROGMEM = "HUMI";
const char mixString[]   	PROGMEM = "MIX";
const char mixdString[]		PROGMEM = "H0:0T";
const char userString[]   	PROGMEM = "USER";
const char growString[]     PROGMEM = "GROWIN";
const char flowString[]     PROGMEM = "FLOWER";
const char dryString[]     	PROGMEM = "DRYING";
const char potsString[]     PROGMEM = "POT";
const char esString[]     	PROGMEM = "EM.STOP";
const char lightString[]	PROGMEM = "LIGHT";
const char aircString[]		PROGMEM = "COOLING";
const char heatString[]		PROGMEM = "HEAT";
const char pumpString[]		PROGMEM = "PUMP";
//const char pidString[]		PROGMEM = "PIDCtrl";

struct PrintableString
{
  char str[9];
  uint16_t posX:4;
  uint16_t posY:3;
  uint16_t metaFlags:6;
  
  static void setString(PrintableString& pstr, uint8_t xPos, uint8_t posY, const char* srcstr, uint8_t metaFlags=0);
  static void setStringFromPRGMEM(PrintableString& pstr, uint8_t xPos, uint8_t posY, const char* srcstr, uint8_t metaFlags=0);
  
  static void copyString(char* deststr, const char* srcstr);
  static void copyStringFromPRGMEM(char* deststr, const char* srcstr);
  
  static void insertNumber(PrintableString& pstr, uint16_t val);
  
  void setMeta(uint8_t posX, uint8_t posY, uint8_t metaFlags = 0);
  void insertBinaryAddress(uint8_t address);
};

struct DisplayHandler
{
  static U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;
  
  static void draw(IPage* const page);
  static void drawHLine(uint8_t row, uint8_t colbeg, uint8_t colend, char c = '-');
  static void drawVLine(uint8_t col, uint8_t rowbeg, uint8_t rowend, char c = '|');
  static void drawPString(PrintableString* const pstr);
    
  static void drawStringFromPRGMEM(const char* srcstr, uint8_t posX, uint8_t posY, bool format_big = false);
  
};


#endif
