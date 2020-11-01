/*generated file sensorpage.cpp*/
#include "sensorpage.hpp"
#include "../../sensors/sensor.hpp"

#include "../../data/data.hpp"
#include "../../data/helper.hpp"
#include "string.hpp"

//ctor
SensorPage::SensorPage(uint8_t sindx)
	: IPage(6, 9/*uint8_t dynelems, uint8_t sz_pstr, uint8_t segaddr*/),
	  sIndx(sindx)
{
	DisplayHandler::u8x8.drawGlyph(15, 2, '%');
	DisplayHandler::u8x8.drawGlyph(15, 4, '%');
	DisplayHandler::u8x8.drawGlyph(15, 5, 'C');
	DisplayHandler::u8x8.drawGlyph(15, 7, 'C');  	
	
	DisplayHandler::u8x8.setInverseFont(true);
	
	DisplayHandler::u8x8.drawGlyph(15, 3, '%');
	DisplayHandler::u8x8.drawGlyph(15, 6, 'C');
	
	DisplayHandler::u8x8.drawGlyph(10, 3, ' ');
	DisplayHandler::u8x8.drawGlyph(10, 4, ' ');
	DisplayHandler::u8x8.draw2x2Glyph(8, 3, '%');
	DisplayHandler::u8x8.drawGlyph(8, 6, 'o');
	DisplayHandler::u8x8.drawGlyph(8, 7, ' ');
	DisplayHandler::u8x8.draw2x2Glyph(9, 6, 'C');
	
	DisplayHandler::drawStringFromPRGMEM(sensString, 0, 0, true);
	DisplayHandler::u8x8.draw2x2Glyph(10, 0, sIndx+48);
	
	DisplayHandler::u8x8.setInverseFont(false);
	
	DisplayHandler::drawStringFromPRGMEM(humiString, 0, 2);
	DisplayHandler::drawStringFromPRGMEM(tempString, 0, 5);
	
	char no[5] = { "00.0" };
	PrintableString::setString(pStrings[0], 11, 2, no, PS_CLICKONLY);
	PrintableString::setString(pStrings[1], 11, 3, no, (PS_CLICKONLY | PS_INVERTED));
	PrintableString::setString(pStrings[2], 11, 4, no, PS_CLICKONLY);
	PrintableString::setString(pStrings[3], 11, 5, no, PS_CLICKONLY);
	PrintableString::setString(pStrings[4], 11, 6, no, (PS_CLICKONLY | PS_INVERTED));
	PrintableString::setString(pStrings[5], 11, 7, no, PS_CLICKONLY);
	PrintableString::setString(pStrings[6], 0, 3, no, (PS_BIG | PS_INVERTED));
	PrintableString::setString(pStrings[7], 0, 6, no, (PS_BIG | PS_INVERTED));
	PrintableString::setString(pStrings[8], 14, 1, "   ");
}

//dtor
SensorPage::~SensorPage()
{
}

//inherited functions
void SensorPage::updateContent() 
{ 
	Sensor *m_sensor = SensorHandle::sensors[sIndx];
	Log *log = m_sensor->log;
  
	PrintableString::insertNumber(pStrings[0], log[0].min);
	PrintableString::insertNumber(pStrings[1], log[0].avrg);
	PrintableString::insertNumber(pStrings[2], log[0].max);
	PrintableString::insertNumber(pStrings[3], log[1].min);
	PrintableString::insertNumber(pStrings[4], log[1].avrg);
	PrintableString::insertNumber(pStrings[5], log[1].max);
	
	PrintableString::insertNumber(pStrings[6], m_sensor->reading.humidity);
	PrintableString::insertNumber(pStrings[7], m_sensor->reading.temperature);
	
	if(m_sensor->reading.state == DHTLIB_OK){
		PrintableString::setString(pStrings[8], 14, 1, "Ok");
	} else {
		PrintableString::setString(pStrings[8], 12, 1, "Err0");
		PrintableString::insertNumber(pStrings[8], m_sensor->reading.state);
	}
} 

void SensorPage::updateDvalue() 
{ 
} 

void SensorPage::updateValue() 
{
	Sensor *s = SensorHandle::sensors[sIndx];
	Log *log;
	uint16_t value;
	if(currCursorIndx < 3){
		log   = s->log;
		value = s->reading.humidity;
	} else {
		log   = &s->log[1];
		value = s->reading.temperature;
	}
	log->init(value);
}

//EOF
