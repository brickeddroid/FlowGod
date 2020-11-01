/*generated file board.hpp*/
#ifndef BOARD_HPP
#define BOARD_HPP

#include "Arduino.h"
#include "timer.hpp"

#define BRD_SENS_R_INTVAL	666

#define BRD_EMSTOP_ACTIVE	128


class IDevice;

class Board 
{
    private:
        //ctor
        Board();
        //dtor
        ~Board();
        
		static uint8_t *segptr;
		static Timer sensT;
		
		static void checkEStop();
		
    protected:
    public:
        
        static IDevice *devcs[8];
        
        static void begin();
        
        static void update();
        
};
#endif //BOARD_HPP
