#ifndef IDEVICE_HPP
#define IDEVICE_HPP

#include "Arduino.h"
#include "timer.hpp"
#include "remotecontrol.hpp"

#define DEVC_ACTIVE		1
#define DEVC_ON			2
#define DEVC_SWITCH		4
#define DEVC_LOCKED		8
#define DEVC_BYTE0		16
#define DEVC_BYTE1		32
#define DEVC_BYTE2		64
#define DEVC_BYTE3		128



class IDevice
{
    public:
	    virtual ~IDevice(){}
	    
	    void update();
	    void switchStatus();
	    void toggleBit(uint8_t bit);
	    void enableBit(uint8_t bit);
	    void disableBit(uint8_t bit);
	    
	    void lock();
	    void unlock();
	
	    virtual void switchOff_devc() = 0;
	    virtual void switchOn_devc() = 0;
	    
	    uint8_t state();
    protected:
	    IDevice(uint8_t segment);
	
	    virtual void update_devc() = 0;
			
	    uint8_t m_state;
	    uint8_t *segptr;
    private:
		uint8_t lmin;
		uint8_t lsec;
	    void updateTimer();
	
	    Timer lockT;
	    Timer lifeT;
};

#endif
