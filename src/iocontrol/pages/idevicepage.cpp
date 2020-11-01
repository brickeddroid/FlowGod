/*generated file idevicepage.cpp*/
#include "idevicepage.hpp"

#include "../../data/data.hpp"
#include "../../data/helper.hpp"
#include "string.hpp"


//ctor
IDevicePage::IDevicePage(uint8_t dynelems, uint8_t sz_pstr, uint8_t dvcindx)
	: IPage(dynelems+3, sz_pstr+3, Data::pd_dvc_seg[dvcindx]/*uint8_t dynelems, uint8_t sz_pstr, uint8_t segaddr*/),
	  dvcIndx(dvcindx)
{
	DisplayHandler::u8x8.drawGlyph(6, 7,  '|');
	PrintableString::setStringFromPRGMEM(pStrings[0], 13, 1, offString, PS_CLICKONLY);
	PrintableString::setString(pStrings[lastCursorIndx], 7, 7, "LT00000h", PS_CLICKONLY);
	//PrintableString::setStringFromPRGMEM(pStrings[lastCursorIndx-1], 0, 7, deactString);
	pStrings[lastCursorIndx-1].setMeta(0, 7, PS_CLICKONLY);
}

//dtor
IDevicePage::~IDevicePage()
{
}

//inherited functions
void IDevicePage::updateContent() 
{
	IDevice *devc = Board::devcs[dvcIndx];
	uint8_t state = devc->state();
	DisplayHandler::u8x8.drawGlyph(12, 0, (state & DEVC_LOCKED) ? '#' : ' ');
	
	PrintableString::copyStringFromPRGMEM(pStrings[0].str, (state & DEVC_ON) ? onString : offString );
	pStrings[0].metaFlags = switchBit(pStrings[0].metaFlags, PS_INVERTED, state & DEVC_ON);
	PrintableString::copyStringFromPRGMEM(pStrings[lastCursorIndx-1].str, (state & DEVC_ACTIVE) ? activeString : deactString );
	pStrings[lastCursorIndx-1].metaFlags = (switchBit(pStrings[lastCursorIndx-1].metaFlags, PS_INVERTED, state & DEVC_ACTIVE) | PS_CLICKONLY);
	
	PrintableString::insertNumber(pStrings[lastCursorIndx], Data::conv8to16(&segptr[PD_DVC_INDX_LIFEHR]));
	pStrings[lastCursorIndx].metaFlags = (switchBit(pStrings[lastCursorIndx].metaFlags, PS_INVERTED, state & DEVC_ON) | PS_CLICKONLY);
	this->updateContent_dpage(); 
} 

void IDevicePage::updateDvalue() 
{
	if(currCursorIndx > 0 && currCursorIndx < lastCursorIndx-1){
		this->updateDvalue_dpage();
	}
} 

void IDevicePage::updateValue() 
{
	if(currCursorIndx == 0){
		Board::devcs[dvcIndx]->switchStatus();
	} else if (currCursorIndx == lastCursorIndx-1){
		Data::pd_array[PD_SEG_ADDR_BRD+PD_BRD_INDX_STATUS] ^= (1 << dvcIndx);
		Board::devcs[dvcIndx]->toggleBit(DEVC_ACTIVE);
	} else if (currCursorIndx == lastCursorIndx){
		Data::conv16to8(0, &segptr[PD_DVC_INDX_LIFEHR]);
	} else {
		this->updateValue_dpage();
	}
	if(currCursorIndx != lastCursorIndx)
		Board::devcs[dvcIndx]->unlock();
}

//EOF
