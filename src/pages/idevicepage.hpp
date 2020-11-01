/*generated file idevicepage.hpp*/
#ifndef IDEVICEPAGE_HPP
#define IDEVICEPAGE_HPP

#include "ipage.hpp"

class IDevicePage : public IPage
{
    private:
    protected:
		uint8_t dvcIndx;
		
        //ctor
        IDevicePage(uint8_t dynelems, uint8_t sz_pstr, uint8_t indx);
        //dtor
        virtual ~IDevicePage();
    public:        
        //inherited functions
	    virtual void updateContent();
		virtual void updateDvalue();
		virtual void updateValue();
		
		virtual void updateContent_dpage() = 0;
		virtual void updateDvalue_dpage() = 0;
		virtual void updateValue_dpage() = 0;
};
#endif //IDEVICEPAGE_HPP
