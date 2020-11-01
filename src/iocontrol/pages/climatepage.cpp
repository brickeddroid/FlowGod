/*generated file heatpage.cpp*/
#include "climatepage.hpp"

#include "../../data/data.hpp"
#include "../../data/helper.hpp"
#include "../../sensors/sensor.hpp"
#include "string.hpp"

//ctor
ClimatePage::ClimatePage(uint8_t dvcindx)
	: 	IDevicePage(5, 6, dvcindx/*uint8_t segaddr, uint8_t dynelems, uint8_t sz_pstr, IDevice *_idevc*/),
		humidifier(dvcindx-2)
{	
	
	DisplayHandler::drawHLine(4, 10, 15, '_');
	
	DisplayHandler::u8x8.drawGlyph(9, 2,  '|');	
	DisplayHandler::u8x8.setInverseFont(true);
	DisplayHandler::u8x8.drawString(11, 2, "Addrs");
	DisplayHandler::u8x8.setInverseFont(false);
	
	PrintableString::setString(pStrings[1], 0, 2, "Sens0");
	PrintableString::setString(pStrings[2], 11, 3, "00000", PS_INVERTED);
	PrintableString::setString(pStrings[3], 0, 5, "00.0C", PS_BIG);
	PrintableString::setString(pStrings[4], 10, 5, "-00.0C");
	PrintableString::setString(pStrings[5], 10, 6, "+00.0C");
	
	PrintableString::setString(pStrings[lastCursorIndx+1], 0, 3, "00.0C", (PS_BIG | PS_INVERTED));
	
	
	if(humidifier){
		DisplayHandler::drawStringFromPRGMEM(humiString, 2, 0, true);	
		pStrings[3].str[4] = '%';
		pStrings[4].str[5] = '%';
		pStrings[5].str[5] = '%';
		pStrings[lastCursorIndx+1].str[4] = '%';
	} else {
		DisplayHandler::drawStringFromPRGMEM(heatString, 2, 0, true);
	}
	DisplayHandler::u8x8.draw2x2Glyph(0, 0,  'C');
}

//dtor
ClimatePage::~ClimatePage()
{
}

//inherited functions
void ClimatePage::updateContent_dpage() 
{
	PrintableString::insertNumber(pStrings[1], segptr[PD_CLIM_INDX_SMIX]);
	pStrings[2].insertBinaryAddress(segptr[PD_CLIM_INDX_ADDR]);
	PrintableString::insertNumber(pStrings[3], convertEncU8toU16(segptr[PD_CLIM_INDX_STPT]));
	PrintableString::insertNumber(pStrings[4], (segptr[PD_CLIM_INDX_DRIFT] & 0x0F) * 10);
	PrintableString::insertNumber(pStrings[5], (segptr[PD_CLIM_INDX_DRIFT] >> 4) * 10); 
	
	Reading r;
	SensorHandle::get(segptr[PD_CLIM_INDX_SMIX], r);
	uint16_t input = (humidifier) ? r.humidity : r.temperature;
	PrintableString::insertNumber(pStrings[lastCursorIndx+1], input);
} 

void ClimatePage::updateDvalue_dpage() 
{
	uint8_t min = 20;
	uint8_t max = 70;
	if(humidifier){
		min = 80;
		max = 160;
	}
	
	switch(currCursorIndx)
	{
		case 1:
		DynamicValue::set(segptr[PD_CLIM_INDX_SMIX], 1, 7);
		break;
		case 2:
		DynamicValue::set(segptr[PD_CLIM_INDX_ADDR], 0, 31);
		break;
		case 3:
		DynamicValue::set(segptr[PD_CLIM_INDX_STPT], min, max);
		break;
		case 4:
		DynamicValue::set(segptr[PD_CLIM_INDX_DRIFT] & 0x0F, 0, 15);
		break;
		case 5:
		DynamicValue::set(segptr[PD_CLIM_INDX_DRIFT] >> 4, 0, 15);
		break;
	} 
} 

void ClimatePage::updateValue_dpage() 
{ 
	switch(currCursorIndx)
	{
		case 1:
		segptr[PD_CLIM_INDX_SMIX] = DynamicValue::value;
		break;
		case 2:
		segptr[PD_CLIM_INDX_ADDR] = DynamicValue::value;
		break;
		case 3:
		segptr[PD_CLIM_INDX_STPT] = DynamicValue::value;
		break;
		case 4:
		segptr[PD_CLIM_INDX_DRIFT] = (segptr[PD_CLIM_INDX_DRIFT] & 0xF0) | DynamicValue::value;
		break;
		case 5:
		segptr[PD_CLIM_INDX_DRIFT] = (DynamicValue::value << 4) | (segptr[PD_CLIM_INDX_DRIFT] & 0x0F);
		break;
	}
}

//EOF
