/*generated file pwmdevice.hpp*/
#ifndef PWMDEVICE_HPP
#define PWMDEVICE_HPP

#include "idevice.hpp"
#include "../sensors/pidhandle.hpp"

#define PWM_TIMEOUT				DEVC_BYTE0
#define PWM_FULLTHROTTLE_ACTIVE	DEVC_BYTE1

#define PWM_FULLTON_INTVAL	SEC_MS(2)
#define PWM_FULLT_INTVAL	HOUR_MS(2)

#define PWM_MINSPEED	50
#define PWM_MAXSPEED	255

#define PWM_TON_INTVAL	HOUR_MS(12)
#define PWM_TOUT_INTVAL	HOUR_MS(2)

class PwmDevice : public IDevice
{
    private:
		uint8_t m_pin;
		
		Timer timeoutT;
		Timer fullthrottleT;
		PidHandle pidhandles[2];
		unsigned long interval;
		
		uint8_t calculateSpeed();
    protected:
	    virtual void update_devc();
    public:
		static void updatePid(IDevice *devc);
		
		uint8_t displayspeed;
        //ctor
        PwmDevice(uint8_t pin, uint8_t segaddr);
        //dtor
        virtual ~PwmDevice();
        
        //inherited functions
	    virtual void switchOff_devc();
	    virtual void switchOn_devc();
};
#endif //PWMDEVICE_HPP
