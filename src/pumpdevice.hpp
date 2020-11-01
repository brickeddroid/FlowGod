/*generated file pumpdevice.hpp*/
#ifndef PUMPDEVICE_HPP
#define PUMPDEVICE_HPP

#include "idevice.hpp"


#define PUMP_INITWATERING	DEVC_BYTE0
#define PUMP_ACTWATERING	DEVC_BYTE1

class PumpDevice : public IDevice
{
    private:
		uint32_t interval;
		Timer wateringT;
		
    protected:
	    virtual void update_devc();
    public:
		uint8_t potcnt;
        //ctor
        PumpDevice();
        //dtor
        virtual ~PumpDevice();
        
        //inherited functions
	    virtual void switchOff_devc();
	    virtual void switchOn_devc();
};
#endif //PUMPDEVICE_HPP
