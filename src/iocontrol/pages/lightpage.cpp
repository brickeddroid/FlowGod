/*generated file lightpage.cpp*/
#include "lightpage.hpp"

#include "../data.hpp"
#include "../helper.hpp"
#include "../lightdevice.hpp"
#include "string.hpp"

//ctor
LightPage::LightPage(uint8_t dvcindx)
	: IDevicePage(7, 9, dvcindx/*uint8_t dynelems, uint8_t sz_pstr, uint8_t dvcindx*/),
		modes({ DRYING_MODE, GROWER_MODE, FLOWER_MODE }),
		mode(0)
{
	//DisplayHandler::drawVLine(8, 2, 6, ':');
  
	DisplayHandler::drawHLine(4, 10, 15, '_');
	
	DisplayHandler::u8x8.setInverseFont(true);
	DisplayHandler::u8x8.draw2x2Glyph(4, 4, ':');
	DisplayHandler::u8x8.drawString(11, 2, "Addrs");
	DisplayHandler::u8x8.setInverseFont(false);
	
	DisplayHandler::drawStringFromPRGMEM(lightString, 0, 0, true);
	DisplayHandler::drawStringFromPRGMEM(onString, 0, 2);
	DisplayHandler::drawStringFromPRGMEM(offString, 0, 3);
	DisplayHandler::drawStringFromPRGMEM(aircString, 0, 6);
	
	PrintableString::setString(pStrings[1], 11, 3, "00000", PS_INVERTED);
	
	DisplayHandler::u8x8.drawGlyph(9, 2, '|');
	DisplayHandler::u8x8.drawGlyph(9, 3, '|');
	DisplayHandler::u8x8.drawGlyph(6, 2, ':');
	DisplayHandler::u8x8.drawGlyph(6, 3, ':');
	
	char str[] = "00";
	PrintableString::setString(pStrings[2], 4, 2, str);
	PrintableString::setString(pStrings[3], 7, 2, str);
	PrintableString::setString(pStrings[4], 4, 3, str);
	PrintableString::setString(pStrings[5], 7, 3, str);
	PrintableString::setStringFromPRGMEM(pStrings[6], 10, 5, flowString);
	PrintableString::setStringFromPRGMEM(pStrings[7], 8, 6, deactString, PS_CLICKONLY);
	PrintableString::setString(pStrings[lastCursorIndx+1], 0, 4, str, PS_BIG | PS_INVERTED);
	PrintableString::setString(pStrings[lastCursorIndx+2], 6, 4, str, PS_BIG | PS_INVERTED);
	
	
}

//dtor
LightPage::~LightPage()
{
}

//inherited functions
void LightPage::updateContent_dpage() 
{ 
	pStrings[1].insertBinaryAddress(segptr[PD_LGHT_INDX_ADDR]);
	PrintableString::insertNumber(pStrings[2], segptr[PD_LGHT_INDX_ONTHR]);
	PrintableString::insertNumber(pStrings[3], segptr[PD_LGHT_INDX_ONTMI]);
	PrintableString::insertNumber(pStrings[4], segptr[PD_LGHT_INDX_OFTHR]);
	PrintableString::insertNumber(pStrings[5], segptr[PD_LGHT_INDX_OFTMI]);
	
	PrintableString::copyStringFromPRGMEM((pStrings[7].str), (Board::devcs[DEVC_INDX_LCOOL]->state() & DEVC_ACTIVE) ? activeString : deactString);
	
	LightDevice *m_light = static_cast<LightDevice*>(Board::devcs[DEVC_INDX_LGHT]);
	
	uint8_t hrs = m_light->getOntimeInterval(true);
	
	PrintableString::insertNumber(pStrings[lastCursorIndx+1], hrs);
	PrintableString::insertNumber(pStrings[lastCursorIndx+2], m_light->getOntimeInterval(false));
	
	if(hrs > 16){
		PrintableString::copyStringFromPRGMEM((pStrings[6].str), growString);
		mode = 2;
	}
	else if(hrs > 9){
		PrintableString::copyStringFromPRGMEM((pStrings[6].str), flowString);
		mode = 1;
	}
	else{
		PrintableString::copyStringFromPRGMEM((pStrings[6].str), dryString);
		mode = 0;
	}
} 

void LightPage::updateDvalue_dpage() 
{
	switch(currCursorIndx){
		case 1:
		DynamicValue::set(segptr[PD_LGHT_INDX_ADDR], 0, 31, 1);
		break;
		case 2:
		DynamicValue::set(segptr[PD_LGHT_INDX_ONTHR], 0, 23, 1);
		break;
		case 3:
		DynamicValue::set(segptr[PD_LGHT_INDX_ONTMI], 0, 59, 1);
		break;
		case 4:
		DynamicValue::set(segptr[PD_LGHT_INDX_OFTHR], 0, 23, 1);
		break;
		case 5:
		DynamicValue::set(segptr[PD_LGHT_INDX_OFTMI], 0, 59, 1);
		break;
		case 6:
		DynamicValue::set(mode, 0, 2);
		break;
	} 
} 

void LightPage::updateValue_dpage() 
{
	switch(currCursorIndx){
		case 1:
		segptr[PD_LGHT_INDX_ADDR] = DynamicValue::value;
		break;
		case 2:
		segptr[PD_LGHT_INDX_ONTHR] = DynamicValue::value;
		break;
		case 3:
		segptr[PD_LGHT_INDX_ONTMI] = DynamicValue::value;
		break;
		case 4:
		segptr[PD_LGHT_INDX_OFTHR] = DynamicValue::value;
		break;
		case 5:
		segptr[PD_LGHT_INDX_OFTMI] = DynamicValue::value;
		break;
		case 6:
		mode = DynamicValue::value;
		segptr[PD_LGHT_INDX_OFTMI] = segptr[PD_LGHT_INDX_ONTMI];
		segptr[PD_LGHT_INDX_OFTHR] = (segptr[PD_LGHT_INDX_ONTHR] + modes[mode] > 23) ? (segptr[PD_LGHT_INDX_ONTHR] + modes[mode] - 24) : segptr[PD_LGHT_INDX_ONTHR] + modes[mode];
		case 7:
		Board::devcs[DEVC_INDX_LCOOL]->toggleBit(DEVC_ACTIVE);
		break;
	}
}

//EOF
