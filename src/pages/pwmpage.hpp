/*generated file pwmpage.hpp*/
#ifndef PWMPAGE_HPP
#define PWMPAGE_HPP

#include "idevicepage.hpp"

class PwmPage : public IDevicePage
{
    private:
        bool tParam;
        void setMixModeString(uint8_t mix, uint8_t strIndx);
    protected:
    public:
        //ctor
        PwmPage(uint8_t dvcindx);
        //dtor
        virtual ~PwmPage();
        
        //inherited functions
		virtual void updateContent_dpage();
		virtual void updateDvalue_dpage();
		virtual void updateValue_dpage();
};
#endif //PWMPAGE_HPP
