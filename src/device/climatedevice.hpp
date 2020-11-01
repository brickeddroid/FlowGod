/*generated file heatdevice.hpp*/
#ifndef CLIMATEDEVICE_HPP
#define CLIMATEDEVICE_HPP

#include "idevice.hpp"

#define CLIM_TIMEOUT	DEVC_BYTE0
#define CLIM_SPT_LOW	DEVC_BYTE1
#define CLIM_SPT_OK		DEVC_BYTE2
#define CLIM_SPT_HIGH	DEVC_BYTE3

#define CLIM_TON_INTVAL		HOUR_MS(10)		//maximum of 10 hours ontime, then 2hours pause
#define CLIM_TOUT_INTVAL	HOUR_MS(2)		//maximum of 10 hours ontime, then 2hours pause

class ClimateDevice : public IDevice
{
    private:
		uint8_t checkSetpoint();
		Timer timeoutT;
		
    protected:
	    virtual void update_devc();
    public:
        //ctor
        ClimateDevice(uint8_t segaddr, bool humdifier = false);
        //dtor
        virtual ~ClimateDevice();
        
        //inherited functions
	    virtual void switchOff_devc();
	    virtual void switchOn_devc();
	    
	    const bool m_humidifier;
};
#endif //HEATDEVICE_HPP
