/*generated file rtcpage.cpp*/
#include "rtcpage.hpp"

#include "../data.hpp"
#include "../helper.hpp"
#include "string.hpp"

#include "../rtc.hpp"

//ctor
RtcPage::RtcPage()
	: IPage(7, 8, PD_SEG_ADDR_BRD/*uint8_t dynelems, uint8_t sz_pstr, uint8_t segaddr*/)
{
	DisplayHandler::drawHLine(2, 0, 15);
	DisplayHandler::drawHLine(5, 0, 15);
	
	DisplayHandler::u8x8.drawGlyph(8, 0, 'o');
	DisplayHandler::u8x8.draw2x2Glyph(9, 0, 'C');
	
	DisplayHandler::u8x8.setInverseFont(true);
	DisplayHandler::u8x8.draw2x2Glyph(4, 3, ':');
	DisplayHandler::u8x8.draw2x2Glyph(10, 3, ':');
	DisplayHandler::u8x8.setInverseFont(false);
	
	DisplayHandler::u8x8.draw2x2Glyph(4, 6, '.');
	DisplayHandler::u8x8.draw2x2Glyph(10, 6, '.');
	
	PrintableString::setString(pStrings[0], 12, 1, "000T");
	
	char str[] = "00";
	PrintableString::setString(pStrings[1], 0, 3, str, (PS_BIG | PS_INVERTED));
	PrintableString::setString(pStrings[2], 6, 3, str, (PS_BIG | PS_INVERTED));
	PrintableString::setString(pStrings[3], 12, 3,str, (PS_BIG | PS_INVERTED));
	PrintableString::setString(pStrings[4], 0, 6, str, PS_BIG);
	PrintableString::setString(pStrings[5], 6, 6, str, PS_BIG);
	PrintableString::setString(pStrings[6], 12, 6,str, PS_BIG);
	PrintableString::setString(pStrings[7], 0, 0, "00.0", PS_BIG);
}

//dtor
RtcPage::~RtcPage()
{
}

//inherited functions
void RtcPage::updateContent() 
{ 
	PrintableString::insertNumber(pStrings[0], Data::conv8to16(&segptr[PD_BRD_INDX_NDAYS]));
	PrintableString::insertNumber(pStrings[1], Rtc::hour);
	PrintableString::insertNumber(pStrings[2], Rtc::min);
	PrintableString::insertNumber(pStrings[3], Rtc::sec);
	PrintableString::insertNumber(pStrings[4], Rtc::date);
	PrintableString::insertNumber(pStrings[5], Rtc::mon);
	PrintableString::insertNumber(pStrings[6], Rtc::year);
	PrintableString::insertNumber(pStrings[7], Rtc::temp);
} 

void RtcPage::updateDvalue() 
{
	switch(currCursorIndx){
		case 0:
		DynamicValue::set(Data::conv8to16(&segptr[PD_BRD_INDX_NDAYS]), 0, 255);
		//DynamicValue::set(Data::readUint16_t(BRD_INDX_NDAYS, segptr), 0, 255, 1);
		break;
		case 1:
		DynamicValue::set(Rtc::hour, 0, 23);
		break;
		case 2:
		DynamicValue::set(Rtc::min, 0, 59);
		break;
		case 3:
		DynamicValue::set(Rtc::sec, 0, 59);
		break;
		case 4:
		DynamicValue::set(Rtc::date, 1, 31);
		break;
		case 5:
		DynamicValue::set(Rtc::mon, 1, 12);
		break;
		case 6:
		DynamicValue::set(Rtc::year, 19, 99);
		break;
	} 
} 

void RtcPage::updateValue() 
{ 
	if(currCursorIndx == 0){
		segptr[PD_BRD_INDX_NDAYS] = DynamicValue::value;
		//Data::conv16to8(DynamicValue::value, &segptr[PD_BRD_INDX_NDAYS]);
	} else {
		Rtc::setTime(DynamicValue::value, currCursorIndx-1);
	}
}

//EOF
