#ifndef IPAGE_HPP
#define IPAGE_HPP

#include "Arduino.h"
#include "../../board.hpp"
#include "../../device/idevice.hpp"
#include "../dynamicvalue.hpp"


#define MENU_AUTO	0
#define MENU_MANUAL	1
#define MENU_PENTRY	2
#define MENU_VSEL	3

#define LAST_PAGE	13

class PrintableString;

//uint8_t menulvl;

class IPage
{
	public:  
		virtual ~IPage(); 
		
		virtual void updateContent() = 0;
		virtual void updateDvalue() = 0;
		virtual void updateValue() = 0;
    
		void update();
		void unlockValue();
		void lockValue();
		void reset();
    
		bool isClickOnly();
    
		const uint8_t lastCursorIndx;
		uint8_t currCursorIndx;
		uint8_t prevCursorIndx;
		
		static uint8_t menulvl;
		
		friend struct DisplayHandler;
	protected:
		IPage(uint8_t dynelems, uint8_t sz_pstr, uint8_t segaddr = 0);
		
		uint8_t *segptr;
		
		PrintableString* pStrings;
		uint8_t size_pstr;
		
		
		//Board* brdptr;
};
#endif
