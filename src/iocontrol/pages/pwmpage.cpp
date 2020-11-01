/*generated file pwmpage.cpp*/
#include "pwmpage.hpp"

#include "../../data/data.hpp"
#include "../../data/helper.hpp"
#include "../../device/pwmdevice.hpp"
#include "string.hpp"
#include "../../sensors/sensor.hpp"

//ctor
PwmPage::PwmPage(uint8_t dvcindx)
	: IDevicePage(10, 12, dvcindx/*uint8_t dynelems, uint8_t sz_pstr, uint8_t dvcindx*/), tParam(false)
{
	DisplayHandler::drawVLine(10, 2, 5);
    DisplayHandler::drawVLine(4, 2, 6);
    
    DisplayHandler::drawHLine(4, 0, 15);
	
    DisplayHandler::u8x8.drawGlyph(4,4, '+');
    DisplayHandler::u8x8.drawGlyph(10,4, '+');  
    DisplayHandler::u8x8.drawGlyph(8,6, '|');
    DisplayHandler::u8x8.drawGlyph(12,6, '|');  
	
    DisplayHandler::u8x8.setInverseFont(true);
    DisplayHandler::u8x8.drawString(0,0, "PWM");
    DisplayHandler::u8x8.drawGlyph(3,0, dvcindx-3+48);
    DisplayHandler::u8x8.setInverseFont(false);
	
	
    DisplayHandler::u8x8.drawGlyph(0, 2, 'V');
    DisplayHandler::drawStringFromPRGMEM(maxString, 1, 2);
    DisplayHandler::drawStringFromPRGMEM(mixString, 0, 3);
	
	
    char c[5] = { "000r" };
    PrintableString::setString(pStrings[1], 6, 2, c);
    PrintableString::setString(pStrings[3], 12, 2, c);
	
    PrintableString::setStringFromPRGMEM(pStrings[2], 5, 3, mixdString);
    PrintableString::setStringFromPRGMEM(pStrings[4], 11, 3, mixdString);
    
    
    PrintableString::setStringFromPRGMEM(pStrings[5], 0, 5, humiString, PS_CLICKONLY);
    PrintableString::setString(pStrings[6], 5, 5, "00.0%");
    PrintableString::setString(pStrings[7], 0, 6, "Sen0");
    PrintableString::setString(pStrings[8], 5, 6, "P00");
    PrintableString::setString(pStrings[9], 9, 6, "I00");
    PrintableString::setString(pStrings[10], 13, 6, "D00");
	
    
    PrintableString::setString(pStrings[lastCursorIndx+1], 4, 0, c, PS_BIG);
    
    PrintableString::setString(pStrings[lastCursorIndx+2], 11, 5, "00.0%");
}

//dtor
PwmPage::~PwmPage()
{
}

//inherited functions
void PwmPage::updateContent_dpage() 
{
	PwmDevice *m_fan = static_cast<PwmDevice*>(Board::devcs[dvcIndx]);
	
  
    DisplayHandler::u8x8.drawGlyph(12, 1, (m_fan->state() & PWM_TIMEOUT) ? '*' : (m_fan->state() & PWM_FULLTHROTTLE_ACTIVE) ? 'F' : ' ');
    
    setMixModeString(segptr[PD_PWM_INDX_MIXN], 2);
    setMixModeString(segptr[PD_PWM_INDX_MIXD], 4);
    PrintableString::insertNumber(pStrings[1], segptr[PD_PWM_INDX_CSN]);
    PrintableString::insertNumber(pStrings[3], segptr[PD_PWM_INDX_CSD]);
    
    PrintableString::insertNumber(pStrings[5], segptr[PD_PWM_INDX_SMIX]);
    
    if(m_fan->state() & DEVC_ON){
		pStrings[lastCursorIndx+1].metaFlags |= PS_INVERTED;
    } else {
		pStrings[lastCursorIndx+1].metaFlags &= ~PS_INVERTED;
    }
    
    uint8_t smix;
    uint8_t *ptr;
    Reading r;
    uint16_t value;
    char c;
    
    if(tParam){
		smix = segptr[PD_PWM_INDX_SMIX] >> 4;
		ptr = &segptr[PD_PWM_INDX_TSP];
		PrintableString::copyStringFromPRGMEM(pStrings[5].str, tempString);
		SensorHandle::get(smix, r);
		value = r.temperature;
		c = 'C';
		pStrings[lastCursorIndx+2].str[4] = 'C';
    } else {
		smix = segptr[PD_PWM_INDX_SMIX] & 0x0F;
		ptr = &segptr[PD_PWM_INDX_HSP];
		PrintableString::copyStringFromPRGMEM(pStrings[5].str, humiString);
		SensorHandle::get(smix, r);
		value = r.humidity;
		c = '%';
	}    
    
    pStrings[6].str[4] = c;
	pStrings[lastCursorIndx+2].str[4] = c;
    PrintableString::insertNumber(pStrings[lastCursorIndx+2], value);
    PrintableString::insertNumber(pStrings[6], convertEncU8toU16(ptr[0]));
    PrintableString::insertNumber(pStrings[7], smix);
    PrintableString::insertNumber(pStrings[8], ptr[1]);
    PrintableString::insertNumber(pStrings[9], ptr[2]);
    PrintableString::insertNumber(pStrings[10], ptr[3]);
    PrintableString::insertNumber(pStrings[lastCursorIndx+1], m_fan->displayspeed);
} 

void PwmPage::updateDvalue_dpage() 
{
	uint8_t *paramptr = &segptr[PD_PWM_INDX_HSP];
	uint8_t smix = segptr[PD_PWM_INDX_SMIX] & 0x0F;
	uint8_t min = 70;
	uint8_t max = 160;
	if(tParam){
		paramptr = &segptr[PD_PWM_INDX_TSP];
		smix = segptr[PD_PWM_INDX_SMIX] >> 4;
		min = 30;
		max = 80;
	}
	switch(currCursorIndx){
		case 1:
		DynamicValue::set(segptr[PD_PWM_INDX_CSN], 45, 255, 5);
		break;
		case 2:
		DynamicValue::set(segptr[PD_PWM_INDX_MIXN], 0, 9, 1);
		break;
		case 3:
		DynamicValue::set(segptr[PD_PWM_INDX_CSD], 45, 255, 5);
		break;
		case 4:
		DynamicValue::set(segptr[PD_PWM_INDX_MIXD], 0, 9, 1);
		break;
		case 6:
		DynamicValue::set(paramptr, min, max, 1);
		break;
		case 7:
		DynamicValue::set(smix, 1, 7, 1);
		break;
		case 8:
		case 9:
		case 10:
		DynamicValue::set(paramptr[currCursorIndx-7], 1, 63, 1);
		break;
	} 
} 

void PwmPage::updateValue_dpage() 
{
	uint8_t *paramptr = &segptr[PD_PWM_INDX_HSP];
	if(tParam){
		paramptr = &segptr[PD_PWM_INDX_TSP];
	}
	switch(currCursorIndx){
		case 1:
		segptr[PD_PWM_INDX_CSN] = (DynamicValue::value == 45) ? 0 : DynamicValue::value;
		break;
		case 2:
		segptr[PD_PWM_INDX_MIXN] = DynamicValue::value;
		break;
		case 3:
		segptr[PD_PWM_INDX_CSD] = (DynamicValue::value == 45) ? 0 : DynamicValue::value;
		break;
		case 4:
		segptr[PD_PWM_INDX_MIXD] = DynamicValue::value;
		break;
		case 5:
		tParam = !tParam;
		break;
		case 6:
		paramptr[0] = DynamicValue::value;
		break;
		case 7:
		if(tParam){
			segptr[PD_PWM_INDX_SMIX] = (DynamicValue::value << 4) | (segptr[PD_PWM_INDX_SMIX] & 0x0F);
		} else {
			segptr[PD_PWM_INDX_SMIX] = (segptr[PD_PWM_INDX_SMIX] & 0xF0) | DynamicValue::value;
		}
		break;
		case 8:
		case 9:
		case 10:
		paramptr[currCursorIndx-7] = DynamicValue::value;
		break;
	} 
}

void PwmPage::setMixModeString(uint8_t mix, uint8_t strIndx)
{
    if(mix > 8){
		PrintableString::copyStringFromPRGMEM(pStrings[strIndx].str, userString);
    }
    else if(mix == 8){
		PrintableString::copyStringFromPRGMEM(pStrings[strIndx].str, tempString);
    }
    else if(mix == 0){
		PrintableString::copyStringFromPRGMEM(pStrings[strIndx].str, humiString);
    }
    else{
		PrintableString::copyStringFromPRGMEM(pStrings[strIndx].str, mixdString);
		PrintableString::insertNumber(pStrings[strIndx], mix + 10*(8-mix));
    }
    if(mix >= 8 || mix == 0){
		DisplayHandler::u8x8.drawGlyph((strIndx&4) ? 11 : 5, 3, ' ');
		pStrings[strIndx].setMeta((strIndx&4) ? 12 : 6, 3, pStrings[strIndx].metaFlags);
    } else {
		pStrings[strIndx].setMeta((strIndx&4) ? 11 : 5, 3, pStrings[strIndx].metaFlags);
    }
}

//EOF
