/*generated file lightdevice.hpp*/
#ifndef LIGHTDEVICE_HPP
#define LIGHTDEVICE_HPP

#include "idevice.hpp"

class LightDevice : public IDevice
{
    private:
		bool isStateNominal();
    protected:
	    virtual void update_devc();
    public:
        //ctor
        LightDevice();
        //dtor
        virtual ~LightDevice();
        
        //inherited functions
	    virtual void switchOff_devc();
	    virtual void switchOn_devc();
	    
	    uint8_t getOntimeInterval(bool isHr);
		
};
#endif //LIGHTDEVICE_HPP
