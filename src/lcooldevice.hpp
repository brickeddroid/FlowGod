/*generated file lcooldevice.hpp*/
#ifndef LCOOLDEVICE_HPP
#define LCOOLDEVICE_HPP

#include "idevice.hpp"

class LCoolDevice : public IDevice
{
    private:
    protected:
	    virtual void update_devc();
    public:
        //ctor
        LCoolDevice();
        //dtor
        virtual ~LCoolDevice();
        
        //inherited functions
	    virtual void switchOff_devc();
	    virtual void switchOn_devc();
};
#endif //LCOOLDEVICE_HPP
