/*generated file iohandle.cpp*/
#include "iohandle.hpp"

#include "rotaryencoder.hpp"

#include "pages/displaypage.hpp"
#include "pages/climatepage.hpp"
#include "pages/lightpage.hpp"
#include "pages/pwmpage.hpp"
#include "pages/pumppage.hpp"
#include "pages/sensorpage.hpp"
#include "pages/rtcpage.hpp"
#include "pages/systempage.hpp"
#include "pages/string.hpp"

#include "data.hpp"
#include "helper.hpp"
#include "sensor.hpp"
#include "rtc.hpp"
#include "memoryhandle.hpp"

Button IOHandle::btn(onClick, onLongPress);

IPage *IOHandle::page = NULL;
uint8_t IOHandle::currPage = 0;
uint8_t IOHandle::prevPage = LAST_PAGE;

Timer IOHandle::refreshT;
Timer IOHandle::switchT;
Timer IOHandle::standbyT;

uint8_t IOHandle::status;
// ctor/dtor
IOHandle::IOHandle()
{
}

// ctor/dtor
IOHandle::~IOHandle()
{
}

void IOHandle::onChange()
{	
	if(DynamicValue::state & DV_CHANGED){
		if(wakeUp()) return;
		DynamicValue::state = 0;
		if(IPage::menulvl < MENU_PENTRY){
			currPage = DynamicValue::value;
		} else if(IPage::menulvl == MENU_PENTRY){
			page->currCursorIndx = DynamicValue::value;
		} else {
			page->updateValue();
		}
	}
}

void IOHandle::onClick()
{
	if(wakeUp()) return;
	if(IPage::menulvl < MENU_PENTRY){
		IPage::menulvl = !IPage::menulvl;
	} else if(IPage::menulvl == MENU_PENTRY){
		if(page->isClickOnly()){
			page->updateValue();
			MemoryHandle::update();
			return;
		}
		page->lockValue();
		page->updateDvalue();
		++IPage::menulvl;
	} else {
		DynamicValue::set(page->currCursorIndx, 0, page->lastCursorIndx);
		--IPage::menulvl;
		MemoryHandle::update();
		page->unlockValue();
	}
}

//void IOHandle::onDoubleClick()
//{
//}

void IOHandle::onLongPress()
{
	if(wakeUp()) return;	
	if(IPage::menulvl < MENU_PENTRY){
		IPage::menulvl = MENU_PENTRY;
		DynamicValue::set(0, 0, page->lastCursorIndx);
	} else {
		IPage::menulvl = MENU_AUTO;
		DynamicValue::set(currPage, 0, LAST_PAGE-1);
		page->reset();
		switchT.start();
	}
}

void IOHandle::begin()
{
	refreshT.start();
	switchT.start();
	standbyT.start();
	
	RotaryEncoder::begin();
    
    DisplayHandler::u8x8.begin(); 
    DisplayHandler::u8x8.clearDisplay(); 
    DisplayHandler::u8x8.setFont( u8x8_font_chroma48medium8_r );
    
    DynamicValue::set(currPage, 0, LAST_PAGE-1);
}

bool IOHandle::wakeUp()
{
	standbyT.start();
	if((status & DEVC_ON) == 0){
		status = DEVC_ON;
		DisplayHandler::u8x8.setPowerSave(0);
		return true;
	}
	return false;
}

void IOHandle::update()
{
	onChange();
	
	if(currPage != prevPage){
		switchT.start();
		prevPage = currPage;
		delete page;
		page = NULL;
		switch(currPage){
			case 0:
				page = new RtcPage();
			break;
			case 1:
			case 2:
			case 3:
				page = new SensorPage(currPage-1);
			break;
			case 4:
				page = new LightPage();
			break;
			case 5:
			case 6:
			case 7:
				page = new PwmPage(currPage-1);
			break;
			case 8:
			case 9:
				page = new ClimatePage(currPage-6);
			break;
			case 10:
				page = new PumpPage();
			break;
			case 11:
				page = new DisplayPage();
			break;
			case 12:
				page = new SystemPage();
			break;
		}
	}
	if(page && refreshT.alarm(DISPLAY_REFRESH)){
		refreshT.start();
		btn.update();
		page->update();
		DisplayHandler::draw(page);
	}
	
	if(IPage::menulvl == MENU_AUTO && switchT.alarm(SEC_MS(Data::pd_array[PD_SEG_ADDR_DISP + PD_DISP_INDX_SWTS]))){
		uint16_t picstates = Data::conv8to16(&Data::pd_array[PD_SEG_ADDR_DISP+PD_DISP_INDX_PICS]);
		do{
			if(++currPage == LAST_PAGE+1){
				currPage = 0;
			}
		} while (((picstates>>(currPage)) & 1)==0);
		//if(++currPage == LAST_PAGE){
		//	currPage = 0;
		//}
		DynamicValue::value = currPage;
	}
	
	uint8_t stdby = Data::pd_array[PD_SEG_ADDR_DISP + PD_DISP_INDX_STBM];
	if(stdby > 0 && standbyT.alarm(MIN_MS((uint32_t)stdby))){
		DisplayHandler::u8x8.setPowerSave(1);
		status = 0;
	}
}

//EOF IOHANDLE_CPP
