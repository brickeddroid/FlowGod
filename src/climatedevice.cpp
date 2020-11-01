/*generated file heatdevice.cpp*/
#include "climatedevice.hpp"

#include "data.hpp"
#include "helper.hpp"
#include "sensor.hpp"
#include "rtc.hpp"

uint8_t ClimateDevice::checkSetpoint()
{
	Reading r;
	SensorHandle::get(segptr[PD_CLIM_INDX_SMIX], r);
	uint16_t setpt = convertEncU8toU16(segptr[PD_CLIM_INDX_STPT]);
	uint8_t lDrift = (segptr[PD_CLIM_INDX_DRIFT] & 0x0F)*5;
	uint8_t uDrift = (segptr[PD_CLIM_INDX_DRIFT] >> 4)*5;
	uint16_t input;
	if(m_humidifier){
		input = r.humidity;
	} else {
		input = r.temperature;
	}
	
	if(setpt-lDrift > input){
		return CLIM_SPT_LOW;
	} else if(setpt+uDrift < input){
		return CLIM_SPT_HIGH;
	} else {
		return CLIM_SPT_OK;
	}
}

//ctor
ClimateDevice::ClimateDevice(uint8_t segaddr, bool humidifier)
	: IDevice(segaddr/*uint8_t segment*/), m_humidifier(humidifier)
{
}

//dtor
ClimateDevice::~ClimateDevice()
{
}

//inherited functions
void ClimateDevice::update_devc()
{
	uint8_t stpt_state = checkSetpoint();
	if(m_state & DEVC_ON){
		if(timeoutT.alarm(CLIM_TON_INTVAL)){
			m_state |= (CLIM_TIMEOUT | DEVC_SWITCH);
			timeoutT.start();
		} else if(stpt_state & CLIM_SPT_HIGH){
			m_state |= DEVC_SWITCH;
		}
	} else {
		if((m_state & CLIM_TIMEOUT) == 0){
			if(stpt_state & CLIM_SPT_LOW){
				m_state |= DEVC_SWITCH;
			}
		} else if(timeoutT.alarm(CLIM_TOUT_INTVAL)){
			m_state &= ~CLIM_TIMEOUT;
			timeoutT.stop();
		}
	}
	
}

void ClimateDevice::switchOff_devc()
{
	if(m_state & CLIM_TIMEOUT){
		timeoutT.start();
	}
	RemoteControl::switchOff(segptr[PD_CLIM_INDX_ADDR]);
}

void ClimateDevice::switchOn_devc()
{
	RemoteControl::switchOn(segptr[PD_CLIM_INDX_ADDR]);
	timeoutT.start();
}

//EOF
