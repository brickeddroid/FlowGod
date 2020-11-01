#ifndef RTC_HPP
#define RTC_HPP

#include "Arduino.h"

#include "HW_AVR_defines.h"

#define DS3231_ADDR_R	0xD1
#define DS3231_ADDR_W	0xD0
#define DS3231_ADDR		0x68
/*
#define SQW_RATE_1		0
#define SQW_RATE_1K		1
#define SQW_RATE_4K		2
#define SQW_RATE_8K		3
*/
#define OUTPUT_SQW		0
#define OUTPUT_INT		1


class Rtc
{
  public:
    
    static void begin();
    static void update();
    
    static void setTime(uint8_t val, uint8_t indx);
    
	static uint8_t hour;
	static uint8_t min;
	static uint8_t sec;
	static uint8_t date;
	static uint8_t mon;
	static uint8_t year;
	
	static uint16_t temp;
	static unsigned long cmillis;
        
  private:
    Rtc() {}
    
    static uint8_t 	_burstArray[7];
    
    static void		_sendStart(byte addr);
    static void		_sendStop();
    static void		_sendAck();
    static void		_sendNack();
    static void		_waitForAck();
    static uint8_t	_readByte();
    static void		_writeByte(uint8_t value);
    static void		_burstRead();
    static uint8_t	_readRegister(uint8_t reg);
    static void 	_writeRegister(uint8_t reg, uint8_t value);
    static uint8_t	_decode(uint8_t value);
    static uint8_t	_decodeH(uint8_t value);
    static uint8_t	_decodeY(uint8_t value);
    static uint8_t	_encode(uint8_t vaule);
};

#endif
