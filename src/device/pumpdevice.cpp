/*generated file pumpdevice.cpp*/
#include "pumpdevice.hpp"

#include "../data/data.hpp"

//ctor
PumpDevice::PumpDevice()
	: IDevice(PD_SEG_ADDR_PUMP/*uint8_t segment*/), potcnt(0)
{
}

//dtor
PumpDevice::~PumpDevice()
{
}

//inherited functions
void PumpDevice::update_devc()
{
	if(m_state & PUMP_INITWATERING){
		m_state &= ~PUMP_INITWATERING;
		m_state ^= PUMP_ACTWATERING;
		
		if(m_state & PUMP_ACTWATERING){
			interval = SEC_MS((uint32_t)segptr[PD_PUMP_INDX_OFFT]*2);
			wateringT.start();
		} else if(m_state & DEVC_ON){
			m_state |= DEVC_SWITCH;
		}
	}
	
	if(m_state & PUMP_ACTWATERING){
		if(wateringT.alarm(interval)){
			m_state |= DEVC_SWITCH;
			m_state &= ~DEVC_LOCKED;
			wateringT.start();		
		}
	} else {
		potcnt = 0;
		wateringT.stop();		
	}	
}

void PumpDevice::switchOff_devc()
{
	digitalWrite(PIN_PUMP, LOW);
	interval = SEC_MS((uint32_t)segptr[PD_PUMP_INDX_OFFT]);
	//wateringT.start();
	if(potcnt == segptr[PD_PUMP_INDX_NPOT]){
		m_state &= ~PUMP_ACTWATERING;
	}
}

void PumpDevice::switchOn_devc()
{
	interval = SEC_MS((uint32_t)segptr[PD_PUMP_INDX_ONT]);
	++potcnt;
	digitalWrite(PIN_PUMP, HIGH);
	//wateringT.start();
}

//EOF
