/*generated file heatpage.hpp*/
#ifndef CLIMATEPAGE_HPP
#define CLIMATEPAGE_HPP

#include "idevicepage.hpp"

class ClimatePage : public IDevicePage
{
    private:
		bool humidifier;
    protected:
    public:
        //ctor
        ClimatePage(uint8_t dvcindx);
        //dtor
        virtual ~ClimatePage();
        
        //inherited functions
		virtual void updateContent_dpage();
		virtual void updateDvalue_dpage();
		virtual void updateValue_dpage();
};
#endif //HEATPAGE_HPP
