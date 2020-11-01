/*generated file pwmdevice.cpp*/
#include "pwmdevice.hpp"

#include "../data/data.hpp"
#include "../data/helper.hpp"
#include "../sensors/sensor.hpp"

void PwmDevice::updatePid(IDevice *devc)
{
	PwmDevice *pwm = static_cast<PwmDevice*>(devc);
	
	
	Reading r;
	uint8_t smix_tmp = pwm->segptr[PD_PWM_INDX_SMIX];
	uint8_t smix =  smix_tmp & 0x0F;
	SensorHandle::get(smix, r);
	
	if((pwm->pidhandles[0].state & PID_ACTIVE) == 0){
		pwm->pidhandles[0].enableAutoMode(true, r.humidity);
		pwm->pidhandles[1].enableAutoMode(true, r.temperature);
	}
	
	pwm->pidhandles[0].calculate(r.humidity, &pwm->segptr[PD_PWM_INDX_HSP]);
	smix_tmp = (smix_tmp & 0xF0 | smix);
	
	smix = smix_tmp >> 4;
	SensorHandle::get(smix, r);
	pwm->pidhandles[1].calculate(r.temperature, &pwm->segptr[PD_PWM_INDX_TSP]);
	pwm->segptr[PD_PWM_INDX_SMIX] = (smix << 4) | (smix_tmp & 0x0F); 
}

uint8_t PwmDevice::calculateSpeed()
{    
	uint8_t uspeed;
	uint8_t mix;
	if(SensorHandle::lightOn()){
		uspeed = segptr[PD_PWM_INDX_CSD];
		mix = segptr[PD_PWM_INDX_MIXD];
	} else {
		uspeed = segptr[PD_PWM_INDX_CSN];
		mix = segptr[PD_PWM_INDX_MIXN];
	}
    
    int16_t hspeed = pidhandles[0].output;
    int16_t tspeed = pidhandles[1].output;
    uint8_t speed = ((tspeed-hspeed)*mix + (hspeed*8))/8;
    return (m_state & PWM_FULLTHROTTLE_ACTIVE) ? PWM_MAXSPEED : (uspeed < speed || mix == 9) ? uspeed : speed;
}

//ctor
PwmDevice::PwmDevice(uint8_t pin, uint8_t segaddr)
	: IDevice(segaddr/*uint8_t segment*/), m_pin(pin), displayspeed(0)
{
}

//dtor
PwmDevice::~PwmDevice()
{
}

//inherited functions
void PwmDevice::update_devc()
{
	uint8_t speed = calculateSpeed();
	
	if(m_state & DEVC_ON){
		if(timeoutT.alarm(PWM_TOUT_INTVAL)){
			m_state |= (PWM_TIMEOUT | DEVC_SWITCH);
			timeoutT.start();
			return;
		}
		if(fullthrottleT.alarm(interval)){
			if(m_state & PWM_FULLTHROTTLE_ACTIVE){
				interval = PWM_FULLT_INTVAL;
			} else {
				interval = PWM_FULLTON_INTVAL;
			}
			m_state ^= PWM_FULLTHROTTLE_ACTIVE;
			fullthrottleT.start();
		}
		if(speed < PWM_MINSPEED){
			m_state |= DEVC_SWITCH;
		} else {
			displayspeed = speed;
			analogWrite(m_pin, speed);
		}
	} else {
		//displayspeed = 0;
		if(m_state & PWM_TIMEOUT){
			uint16_t setpt = convertEncU8toU16(segptr[PD_PWM_INDX_TSP]);
			Reading r;
			uint8_t mix = segptr[PD_PWM_INDX_SMIX]>>4;
			SensorHandle::get(mix, r);
			if(timeoutT.alarm(PWM_TOUT_INTVAL) || r.temperature >= setpt + 10){
				m_state &= ~PWM_TIMEOUT;
				timeoutT.stop();
			}
		} else if(speed >= PWM_MINSPEED){
			m_state |= DEVC_SWITCH;
		}
	}
}

void PwmDevice::switchOff_devc()
{
	analogWrite(m_pin, 0);
	displayspeed = 0;
	fullthrottleT.stop();
}

void PwmDevice::switchOn_devc()
{
	timeoutT.start();
	fullthrottleT.start();
	analogWrite(m_pin, PWM_MAXSPEED);
	displayspeed = PWM_MAXSPEED;
	m_state |= PWM_FULLTHROTTLE_ACTIVE;
	interval = PWM_FULLTON_INTVAL;
}

//EOF
