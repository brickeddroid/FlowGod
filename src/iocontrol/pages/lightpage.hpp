/*generated file lightpage.hpp*/
#ifndef LIGHTPAGE_HPP
#define LIGHTPAGE_HPP

#include "idevicepage.hpp"

#define GROWER_MODE 18
#define FLOWER_MODE 12
#define DRYING_MODE 0

class LightPage : public IDevicePage
{
    private:
		const uint8_t modes[3];
		uint8_t mode;
    protected:
    public:
        //ctor
        LightPage(uint8_t dvcindx = 0);
        //dtor
        virtual ~LightPage();
        
        //inherited functions
		virtual void updateContent_dpage();
		virtual void updateDvalue_dpage();
		virtual void updateValue_dpage();
};
#endif //LIGHTPAGE_HPP
