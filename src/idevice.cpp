/*generated file idevice.cpp*/
#include "idevice.hpp"

#include "data.hpp"
#include "helper.hpp"
#include "sensor.hpp"
#include "rtc.hpp"

void IDevice::update()
{
	if(m_state & DEVC_ACTIVE){
		if((m_state & (DEVC_SWITCH | DEVC_LOCKED)) == DEVC_SWITCH){
			if(m_state & DEVC_ON){
				this->switchOff_devc();
			} else {
				this->switchOn_devc();
			}
			m_state &= ~DEVC_SWITCH;
			lock();
			m_state ^= DEVC_ON;
		} else {
			this->update_devc();
			updateTimer();
		}
		
	} else if(m_state & DEVC_ON){
		this->switchOff_devc();
		m_state &= ~DEVC_ON;
	}
}

void IDevice::switchStatus()
{
	enableBit(DEVC_SWITCH | DEVC_ACTIVE);
	disableBit(DEVC_LOCKED);
}

void IDevice::toggleBit(uint8_t bit)
{
	m_state ^= bit;
}

void IDevice::enableBit(uint8_t bit)
{
	m_state |= bit;
}

void IDevice::disableBit(uint8_t bit)
{
	m_state &= (~bit);
}

uint8_t IDevice::state()
{
    return m_state;
}

// ctor/dtor
IDevice::IDevice(uint8_t segaddr)
	: segptr(&Data::pd_array[segaddr]), m_state(0), lmin(0), lsec(0)
{
}

void IDevice::updateTimer()
{
	if(m_state & DEVC_LOCKED && lockT.alarm(MIN_MS(3))){
		unlock();
	}
	
	if(m_state & DEVC_ON && lifeT.alarm(SEC_MS(1))){
		if(++lsec == 60){
			lsec = 0;
			if(++lmin == 60){
				lmin = 0;
				uint16_t lifehr = Data::conv8to16(&segptr[PD_DVC_INDX_LIFEHR]);
				Data::conv16to8(++lifehr, &segptr[PD_DVC_INDX_LIFEHR]);
			}
		}
	}
}

void IDevice::lock()
{
	m_state |= DEVC_LOCKED;
	lockT.start();
}

void IDevice::unlock()
{
	m_state &= ~DEVC_LOCKED;
	lockT.stop();
}

//EOF IDEVICE_CPP
