/*generated file iohandle.hpp*/
#ifndef IOHANDLE_HPP
#define IOHANDLE_HPP

#include "Arduino.h"
#include "button.hpp"
#include "../sensors/timer.hpp"

class IPage;

#define DISPLAY_REFRESH	50


class IOHandle 
{
    private:
        //ctor
        IOHandle();
        //dtor
        ~IOHandle();
        
        static void onChange();
        
		static void onClick();
		//static void onDoubleClick();
		static void onLongPress();
		
		static bool wakeUp();
		
		static Button btn;
		
		static IPage *page;
		static uint8_t currPage;
		static uint8_t prevPage;
		
		static Timer refreshT;		
		static Timer switchT;
		static Timer standbyT;
		
		static uint8_t status;
    protected:
    public:
        static void begin();
        static void update();

};
#endif //IOHANDLE_HPP
