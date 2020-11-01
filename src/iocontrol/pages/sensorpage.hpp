/*generated file sensorpage.hpp*/
#ifndef SENSORPAGE_HPP
#define SENSORPAGE_HPP

#include "ipage.hpp"

class Sensor;

class SensorPage : public IPage
{
    private:
		uint8_t sIndx;
    protected:
    public:
        //ctor
        SensorPage(uint8_t sIndx);
        //dtor
        virtual ~SensorPage();
        
        //inherited functions
	    virtual void updateContent();
		virtual void updateDvalue();
		virtual void updateValue();
};
#endif //SENSORPAGE_HPP
