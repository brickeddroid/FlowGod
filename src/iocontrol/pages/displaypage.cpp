/*generated file displaypage.cpp*/
#include "displaypage.hpp"

#include "../../data/data.hpp"
#include "../../data/helper.hpp"
#include "string.hpp"

//ctor
DisplayPage::DisplayPage()
	: IPage(LAST_PAGE+2, LAST_PAGE+2, PD_SEG_ADDR_DISP/*uint8_t dynelems, uint8_t sz_pstr, uint8_t segaddr*/)
{
	DisplayHandler::drawVLine(7, 2, 3, ':');
		
	DisplayHandler::drawStringFromPRGMEM(dispString, 0, 0, true);
	
	DisplayHandler::u8x8.drawString(0, 2, "UmschZ.");
	DisplayHandler::u8x8.drawString(0, 3, "Standby");
	PrintableString::setString(pStrings[0], 11, 2, "00Sek");
	PrintableString::setString(pStrings[1], 11, 3, "00Min");
	
	uint16_t picstate = Data::conv8to16(&segptr[PD_DISP_INDX_PICS]);
	
	for(uint8_t u = 0; u < LAST_PAGE; u++)
	{
		PrintableString::setString(pStrings[u+2], (u%5)*3, 5+u/5, "00", PS_CLICKONLY);
		PrintableString::insertNumber(pStrings[u+2], u);
		
		pStrings[u+2].metaFlags ^= (-((picstate>>u) & 1) ^ pStrings[u+2].metaFlags) & (1U << 2); 
	}
}

//dtor
DisplayPage::~DisplayPage()
{
}

//inherited functions
void DisplayPage::updateContent() 
{
	uint8_t swsec = segptr[PD_DISP_INDX_SWTS];
	uint8_t sbmin = segptr[PD_DISP_INDX_STBM];
	PrintableString::insertNumber(pStrings[0], swsec);
	if(sbmin){
		PrintableString::setString(pStrings[1], 11, 3, "00Min", pStrings[1].metaFlags);
		PrintableString::insertNumber(pStrings[1], sbmin);
	}
	else{
		PrintableString::setString(pStrings[1], 13, 3, "Aus", pStrings[1].metaFlags);
		DisplayHandler::u8x8.drawString(11, 3, "  ");
	}
}

void DisplayPage::updateDvalue() 
{ 
	if(currCursorIndx == 0){
		DynamicValue::set(segptr[PD_DISP_INDX_SWTS], 1, 15, 1);
	} else if(currCursorIndx == 1){
		DynamicValue::set(segptr[PD_DISP_INDX_STBM], 0, 15, 1);
	}
}
 
void DisplayPage::updateValue() 
{
	if(currCursorIndx == 0){
		segptr[PD_DISP_INDX_SWTS] = DynamicValue::value;
	} else if(currCursorIndx == 1){
		segptr[PD_DISP_INDX_STBM] = DynamicValue::value;
	} else {
		uint16_t pics = Data::conv8to16(&segptr[PD_DISP_INDX_PICS]);
		uint16_t pic = (1<<(currCursorIndx-2)); 
		if(pics & pic){
		    if(pics & (~pic)){
				pics &= ~pic;
				pStrings[currCursorIndx].metaFlags &= ~PS_INVERTED;
			} 
		} else {
			pics |= pic;
			pStrings[currCursorIndx].metaFlags |= PS_INVERTED;
		}			
		Data::conv16to8(pics, &segptr[PD_DISP_INDX_PICS]);
	}	 
}

//EOF
