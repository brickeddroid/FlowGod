/*generated file pumppage.hpp*/
#ifndef PUMPPAGE_HPP
#define PUMPPAGE_HPP

#include "idevicepage.hpp"
#include "../../device/pumpdevice.hpp"

class PumpPage : public IDevicePage
{
    private:
    protected:
    public:
        //ctor
        PumpPage(uint8_t dvcindx = 7);
        //dtor
        virtual ~PumpPage();
        
        //inherited functions
		virtual void updateContent_dpage();
		virtual void updateDvalue_dpage();
		virtual void updateValue_dpage();
};
#endif //PUMPPAGE_HPP
