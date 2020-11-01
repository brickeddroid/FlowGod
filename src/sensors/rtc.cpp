#include "rtc.hpp"

#include "HW_AVR.h"
#include "../data/data.hpp"

#define REG_SEC		0x00
#define REG_MIN		0x01
#define REG_HOUR	0x02
#define REG_DOW		0x03
#define REG_DATE	0x04
#define REG_MON		0x05
#define REG_YEAR	0x06
#define REG_AGING	0x10
#define REG_TEMPM	0x11
#define REG_TEMPL	0x12

uint8_t Rtc::hour = 0;
uint8_t Rtc::min = 	0;
uint8_t Rtc::sec = 	0;
uint8_t Rtc::date = 0;		// due initializing reasons, otherwise daycount is counted up by one at every restart
uint8_t Rtc::mon = 	1;
uint8_t Rtc::year = 19;

uint16_t Rtc::temp = 0;
unsigned long Rtc::cmillis;

uint8_t Rtc::_burstArray[7];

void Rtc::update()
{
	cmillis = millis();

	static uint8_t ucnt = 0;
	if(++ucnt % 4 == 0){
		_burstRead();
		sec  = _decode(_burstArray[0]);
		min  = _decode(_burstArray[1]);
		hour = _decodeH(_burstArray[2]);
		date = _decode(_burstArray[4]);
		mon  = _decode(_burstArray[5]);
		year = _decodeY(_burstArray[6]);
	
		//read temperature
		uint8_t _msb = _readRegister(REG_TEMPM);
		uint8_t _lsb = _readRegister(REG_TEMPL);
		temp = (uint16_t)((_msb + ((_lsb >> 7) * 0.5f))*10);
	}
}


void Rtc::setTime(uint8_t value, uint8_t indx)
{
  _writeRegister(indx, _encode(value));
}

void Rtc::_sendStart(byte addr)
{
  pinMode(PIN_RTC_SDA, OUTPUT);
  digitalWrite(PIN_RTC_SDA, HIGH);
  digitalWrite(PIN_RTC_SCL, HIGH);
  digitalWrite(PIN_RTC_SDA, LOW);
  digitalWrite(PIN_RTC_SCL, LOW);
  shiftOut(PIN_RTC_SDA, PIN_RTC_SCL, MSBFIRST, addr);
}

void Rtc::_sendStop()
{
  pinMode(PIN_RTC_SDA, OUTPUT);
  digitalWrite(PIN_RTC_SDA, LOW);
  digitalWrite(PIN_RTC_SCL, HIGH);
  digitalWrite(PIN_RTC_SDA, HIGH);
  pinMode(PIN_RTC_SDA, INPUT);
}

void Rtc::_sendNack()
{
  pinMode(PIN_RTC_SDA, OUTPUT);
  digitalWrite(PIN_RTC_SCL, LOW);
  digitalWrite(PIN_RTC_SDA, HIGH);
  digitalWrite(PIN_RTC_SCL, HIGH);
  digitalWrite(PIN_RTC_SCL, LOW);
  pinMode(PIN_RTC_SDA, INPUT);
}

void Rtc::_sendAck()
{
  pinMode(PIN_RTC_SDA, OUTPUT);
  digitalWrite(PIN_RTC_SCL, LOW);
  digitalWrite(PIN_RTC_SDA, LOW);
  digitalWrite(PIN_RTC_SCL, HIGH);
  digitalWrite(PIN_RTC_SCL, LOW);
  pinMode(PIN_RTC_SDA, INPUT);
}

void Rtc::_waitForAck()
{
  pinMode(PIN_RTC_SDA, INPUT);
  digitalWrite(PIN_RTC_SCL, HIGH);
  while (digitalRead(PIN_RTC_SDA)==HIGH) {}
  digitalWrite(PIN_RTC_SCL, LOW);
}

uint8_t Rtc::_readByte()
{
  pinMode(PIN_RTC_SDA, INPUT);

  uint8_t value = 0;
  uint8_t currentBit = 0;

  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(PIN_RTC_SCL, HIGH);
    currentBit = digitalRead(PIN_RTC_SDA);
    value |= (currentBit << 7-i);
    delayMicroseconds(1);
    digitalWrite(PIN_RTC_SCL, LOW);
  }
  return value;
}

void Rtc::_writeByte(uint8_t value)
{
  pinMode(PIN_RTC_SDA, OUTPUT);
  shiftOut(PIN_RTC_SDA, PIN_RTC_SCL, MSBFIRST, value);
}

uint8_t	Rtc::_decode(uint8_t value)
{
  uint8_t decoded = value & 127;
  decoded = (decoded & 15) + 10 * ((decoded & (15 << 4)) >> 4);
  return decoded;
}

uint8_t Rtc::_decodeH(uint8_t value)
{
  if (value & 128)
    value = (value & 15) + (12 * ((value & 32) >> 5));
  else
    value = (value & 15) + (10 * ((value & 48) >> 4));
  return value;
}

uint8_t	Rtc::_decodeY(uint8_t value)
{
  uint8_t decoded = (value & 15) + 10 * ((value & (15 << 4)) >> 4);
  return decoded;
}

uint8_t Rtc::_encode(uint8_t value)
{
  uint8_t encoded = ((value / 10) << 4) + (value % 10);
  return encoded;
}



