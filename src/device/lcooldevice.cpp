/*generated file lcooldevice.cpp*/
#include "lcooldevice.hpp"

#include "../data/data.hpp"
#include "../data/helper.hpp""
#include "../sensors/sensor.hpp"

//ctor
LCoolDevice::LCoolDevice()
	: IDevice(PD_SEG_ADDR_LFAN/*uint8_t segment*/)
{
}

//dtor
LCoolDevice::~LCoolDevice()
{
}

//inherited functions
void LCoolDevice::update_devc()
{
	if(XOR(SensorHandle::lightOn(), m_state & DEVC_ON)){
		m_state |= DEVC_SWITCH;
		m_state &= ~DEVC_LOCKED;
	}
}

void LCoolDevice::switchOff_devc()
{
	digitalWrite(PIN_LCOOL, LOW);
}

void LCoolDevice::switchOn_devc()
{
	digitalWrite(PIN_LCOOL, HIGH);
}

//EOF
