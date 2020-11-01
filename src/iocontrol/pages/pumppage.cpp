/*generated file pumppage.cpp*/
#include "pumppage.hpp"

#include "../../data/data.hpp"
#include "../../data/helper.hpp"
#include "string.hpp"

//ctor
PumpPage::PumpPage(uint8_t dvcindx)
	: IDevicePage(4, 4, dvcindx/*uint8_t dynelems, uint8_t sz_pstr, uint8_t dvcindx*/)
{
	DisplayHandler::u8x8.setInverseFont(true);
	DisplayHandler::drawStringFromPRGMEM(pumpString, 0, 0, true);
	DisplayHandler::u8x8.setInverseFont(false);
	DisplayHandler::u8x8.drawGlyph(7, 4, '|');
	
	
	DisplayHandler::drawStringFromPRGMEM(potsString, 0, 3);
	DisplayHandler::drawStringFromPRGMEM(onString, 1, 4);
	DisplayHandler::drawStringFromPRGMEM(offString, 8, 4);
	
	
	//WateringCtrl
	PrintableString::setString(pStrings[1], 3, 2, "00/00", PS_BIG);
	PrintableString::setString(pStrings[2], 4, 4, "00s");
	PrintableString::setString(pStrings[3], 12, 4, "00s");
	PrintableString::setString(pStrings[4],  3, 5, "START", (PS_BIG | PS_CLICKONLY));
}

//dtor
PumpPage::~PumpPage()
{
}

//inherited functions
void PumpPage::updateContent_dpage() 
{ 
	PumpDevice *m_pump = static_cast<PumpDevice*>(Board::devcs[dvcIndx]);
	//WateringCtrl
	//DisplayHandler::u8x8.drawGlyph(12, 1, (m_pump->state & PUMP_ACTWAT) ? 'W' : ' ');
	PrintableString::insertNumber(pStrings[1], (uint16_t)m_pump->potcnt*100 + segptr[PD_PUMP_INDX_NPOT]);
	PrintableString::insertNumber(pStrings[2], segptr[PD_PUMP_INDX_ONT]);
	PrintableString::insertNumber(pStrings[3], segptr[PD_PUMP_INDX_OFFT]);
	if(m_pump->state() & PUMP_ACTWATERING){
		DisplayHandler::u8x8.drawGlyph(3, 5, ' ');
		DisplayHandler::u8x8.drawGlyph(3, 6, ' ');
		DisplayHandler::u8x8.drawGlyph(12, 5, ' ');
		DisplayHandler::u8x8.drawGlyph(12, 6, ' ');
		PrintableString::copyString(pStrings[4].str, "STOP");
		pStrings[4].posX = 4;
	} else {
		PrintableString::copyString(pStrings[4].str, "START");
		pStrings[4].posX = 3;
	}
	//pStrings[4].posX = (m_pump->state() & PUMP_ACTWATERING) ? 4 : 3;
} 

void PumpPage::updateDvalue_dpage() 
{
	switch(currCursorIndx){
		case 1:
		DynamicValue::set(segptr[PD_PUMP_INDX_NPOT], 1, 40, 1);
		break;
		case 2:
		DynamicValue::set(segptr[PD_PUMP_INDX_ONT], 4, 40, 1);
		break;
		case 3:
		DynamicValue::set(segptr[PD_PUMP_INDX_OFFT], 2, 20, 1);
		break;
	} 
} 

void PumpPage::updateValue_dpage() 
{ 
	switch(currCursorIndx){
		case 1:
		segptr[PD_PUMP_INDX_NPOT] = DynamicValue::value;
		break;
		case 2:
		segptr[PD_PUMP_INDX_ONT] = DynamicValue::value;
		break;
		case 3:
		segptr[PD_PUMP_INDX_OFFT] = DynamicValue::value;
		break;
		case 4:
		Board::devcs[dvcIndx]->toggleBit(PUMP_INITWATERING);
		break;
	} 
}

//EOF
