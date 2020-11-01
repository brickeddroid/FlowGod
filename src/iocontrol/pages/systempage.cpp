/*generated file systempage.cpp*/
#include "systempage.hpp"

#include "../data.hpp"
#include "../helper.hpp"
#include "../memoryhandle.hpp"
#include "string.hpp"

//ctor
SystemPage::SystemPage()
	: IPage(20, 20, PD_SEG_ADDR_BRD/*uint8_t dynelems, uint8_t sz_pstr, uint8_t segaddr*/)
{
	DisplayHandler::u8x8.setInverseFont(true);
	DisplayHandler::drawStringFromPRGMEM(sysString, 0, 0, true);
	DisplayHandler::u8x8.setInverseFont(false);
	
	DisplayHandler::drawVLine(7, 2, 4, ':');
	DisplayHandler::drawStringFromPRGMEM(addrString, 0, 2);
	DisplayHandler::drawStringFromPRGMEM(esString, 0, 3);
	DisplayHandler::drawStringFromPRGMEM(offString, 0, 4);
	DisplayHandler::u8x8.drawString(4, 4, "AT");
	
	PrintableString::setString(pStrings[0], 12, 1, "MSL0");
	PrintableString::setString(pStrings[1], 11, 2, "00000");
	PrintableString::setStringFromPRGMEM(pStrings[2], 10, 3, deactString);
	PrintableString::setString(pStrings[3], 11, 4, "00.0C");
	
	DisplayHandler::u8x8.setInverseFont(true);
	DisplayHandler::u8x8.drawString(0, 5, "|L|K|H|D|E|D|I|P");
	DisplayHandler::u8x8.drawString(0, 6, "A");
	DisplayHandler::u8x8.drawString(0, 7, "O");
	DisplayHandler::u8x8.setInverseFont(false);
	
	
	for(int i = 0; i < 8; i++){
		PrintableString::setString(pStrings[i*2+4], i*2+1, 6, (Board::devcs[i]->state() & DEVC_ACTIVE) ? "x" : "-", PS_CLICKONLY);
		PrintableString::setString(pStrings[i*2+5], i*2+1, 7, (Board::devcs[i]->state() & DEVC_ON) ? "x" : "-", PS_CLICKONLY);
	}
}

//dtor
SystemPage::~SystemPage()
{
}

//inherited functions
void SystemPage::updateContent() 
{ 
	PrintableString::insertNumber(pStrings[0], MemoryHandle::slot);
	pStrings[1].insertBinaryAddress(segptr[PD_BRD_INDX_ADDR]);
	PrintableString::copyStringFromPRGMEM(pStrings[2].str, (segptr[PD_BRD_INDX_ESTOP] & BRD_EMSTOP_ACTIVE) ? activeString : deactString );
	PrintableString::insertNumber(pStrings[3], (uint16_t)(segptr[PD_BRD_INDX_ESTOP] & (~BRD_EMSTOP_ACTIVE))*5);
	
	for(int i = 0; i < 8; i++){
		pStrings[i*2+4].str[0] = (Board::devcs[i]->state() & DEVC_ACTIVE) ? 'X' : '-';
		pStrings[i*2+5].str[0] = (Board::devcs[i]->state() & DEVC_ON) ? 'X': '-';
	}
} 

void SystemPage::updateDvalue() 
{
	if(currCursorIndx == 0){
		DynamicValue::set(MemoryHandle::slot, 0, 9);
	} else if(currCursorIndx == 1){
		DynamicValue::set(segptr[PD_BRD_INDX_ADDR], 0, 31);
	} else if(currCursorIndx == 3) {
		DynamicValue::set(segptr[PD_BRD_INDX_ESTOP] & ~BRD_EMSTOP_ACTIVE, 70, 120);
	}
} 

void SystemPage::updateValue() 
{ 
	if(currCursorIndx == 0){
		MemoryHandle::slot = DynamicValue::value;
		MemoryHandle::initialize();
	} else if(currCursorIndx == 1){
		segptr[PD_BRD_INDX_ADDR] = DynamicValue::value;
	} else if(currCursorIndx == 2){
		segptr[PD_BRD_INDX_ESTOP] ^= BRD_EMSTOP_ACTIVE;
	} else if(currCursorIndx == 3){
		segptr[PD_BRD_INDX_ESTOP] = (segptr[PD_BRD_INDX_ESTOP] & BRD_EMSTOP_ACTIVE) | DynamicValue::value;
	} else {
		uint8_t cindx = (currCursorIndx-4)/2;
		if(currCursorIndx % 2){
			Board::devcs[cindx]->switchStatus();
		} else {
			Data::pd_array[PD_SEG_ADDR_BRD+PD_BRD_INDX_STATUS] ^= (1 << cindx);
			Board::devcs[cindx]->toggleBit(DEVC_ACTIVE);
		}
	}
}

//EOF
