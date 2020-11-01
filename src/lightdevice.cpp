/*generated file lightdevice.cpp*/
#include "lightdevice.hpp"

#include "data.hpp"
#include "helper.hpp"
#include "rtc.hpp"
#include "sensor.hpp"

bool LightDevice::isStateNominal()
{
  uint16_t time = (uint16_t)Rtc::hour * 60 + Rtc::min;
  uint16_t on_t = (uint16_t)segptr[PD_LGHT_INDX_ONTHR]  * 60 + segptr[PD_LGHT_INDX_ONTMI];
  uint16_t of_t = (uint16_t)segptr[PD_LGHT_INDX_OFTHR]  * 60 + segptr[PD_LGHT_INDX_OFTMI];
  
  uint8_t tmp_state = 0;
  if(((time < of_t) && (on_t > of_t || on_t <= time)) ||
      (of_t < on_t && on_t <= time))
  {
    tmp_state |= DEVC_ON;
  }
  
  return ((m_state & DEVC_ON) == tmp_state);  
}

//ctor
LightDevice::LightDevice()
	: IDevice(PD_SEG_ADDR_LGHT/*uint8_t segment*/)
{
}

//dtor
LightDevice::~LightDevice()
{
}

//inherited functions
void LightDevice::update_devc()
{
	if(SensorHandle::lightOn()){
		m_state |= DEVC_ON;
	} else {
		m_state &= ~DEVC_ON;
	}
  
	if(!isStateNominal()){
		m_state |= DEVC_SWITCH;
	} else {
		m_state &= ~DEVC_SWITCH;
	}
}

void LightDevice::switchOff_devc()
{

	RemoteControl::switchOff(segptr[PD_LGHT_INDX_ADDR]);
}

void LightDevice::switchOn_devc()
{
	RemoteControl::switchOn(segptr[PD_LGHT_INDX_ADDR]);
}

uint8_t LightDevice::getOntimeInterval(bool isHr)
{
    uint8_t hr = 0;
    uint8_t offtime_min = segptr[PD_LGHT_INDX_OFTMI];
    uint8_t ontime_min = segptr[PD_LGHT_INDX_ONTMI];
    
    if(isHr){
		uint8_t offtime_hr = segptr[PD_LGHT_INDX_OFTHR];
		uint8_t ontime_hr = segptr[PD_LGHT_INDX_ONTHR];
		hr = (offtime_hr < ontime_hr) ? offtime_hr +24 - ontime_hr : offtime_hr-ontime_hr;
		if(offtime_min < ontime_min) hr--;
		return hr;
    }
    else {
		return (offtime_min < ontime_min) ? offtime_min +60 - ontime_min : offtime_min-ontime_min;
    }
}

//EOF
