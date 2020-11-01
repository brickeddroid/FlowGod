/*generated file board.cpp*/
#include "board.hpp"

#include "iocontrol/iohandle.hpp"

#include "data/data.hpp"
#include "data/helper.hpp"
#include "sensors/sensor.hpp"
#include "sensors/rtc.hpp"
#include "data/memoryhandle.hpp"

#include "device/lightdevice.hpp"
#include "device/lcooldevice.hpp"
#include "device/climatedevice.hpp"
#include "device/pwmdevice.hpp"
#include "device/pumpdevice.hpp"

Timer Board::sensT;

uint8_t *Board::segptr = &Data::pd_array[PD_SEG_ADDR_BRD];
IDevice *Board::devcs[8] = { 	
	new LightDevice(), 
	new LCoolDevice(), 
	new ClimateDevice(PD_SEG_ADDR_HEAT),
	new ClimateDevice(PD_SEG_ADDR_HUMI, true),
	new PwmDevice(PIN_PWM0, PD_SEG_ADDR_PWM0),
	new PwmDevice(PIN_PWM1, PD_SEG_ADDR_PWM1),
	new PwmDevice(PIN_PWM2, PD_SEG_ADDR_PWM2),
	new PumpDevice()
};
								

//ctor
Board::Board()
{
}

//dtor
Board::~Board()
{
}

void Board::checkEStop()
{
	uint8_t active = segptr[PD_BRD_INDX_ESTOP];
	if(active & BRD_EMSTOP_ACTIVE){
		Reading r;
		uint8_t mix = SENS_MIX_ALL;
		SensorHandle::get(mix, r);
		if((uint16_t)(active & ~BRD_EMSTOP_ACTIVE)*5 < r.temperature){
			for(uint8_t u = 0; u < 8; u++){
				if(devcs[u]->state() & DEVC_ON){
					devcs[u]->switchStatus();
				}	
			}
			RemoteControl::switchOff(segptr[PD_BRD_INDX_ADDR]);
		}
	}
}


void Board::begin()
{
	TCCR1B = TCCR1B & 0b11111000 | 0x01;   // Set Timer1 (Pin 9 & 10) to 31kHz, to avoid high frequent noises from fans on pwm pins(9,10,11)
	TCCR2B = TCCR2B & 0b11111000 | 0x01;   // Set Timer2 (Pin 3 & 11) to 31kHz, to avoid high frequent noises from fans on pwm pins(9,10,11)
	
	MemoryHandle::begin();
	
	for(uint8_t u = 0; u < DEVC_INDX_PUMP+1; u++){
		if((segptr[PD_BRD_INDX_STATUS] >> u) & 1){
			devcs[u]->enableBit(DEVC_ACTIVE);
		}
	}
	
	RemoteControl::begin(&segptr[PD_BRD_INDX_ADDR]);
	Rtc::begin();
	Rtc::update();
	
	IOHandle::begin();
	sensT.start();
}

void Board::update()
{
	Rtc::update();
	
	static uint8_t fanIndx = DEVC_INDX_PWM0;
	static uint8_t dvcIndx = 0;
	static uint8_t date = 0;
	
	if(sensT.alarm(BRD_SENS_R_INTVAL)){
		SensorHandle::update();
		sensT.start();
		
		if(++fanIndx == DEVC_INDX_PUMP) fanIndx = DEVC_INDX_PWM0;
		
		PwmDevice::updatePid(devcs[fanIndx]);
		
		if(date > 0 && date != Rtc::date){
			uint16_t dcnt = Data::conv8to16(&segptr[PD_BRD_INDX_NDAYS]);
			Data::conv16to8(++dcnt, &segptr[PD_BRD_INDX_NDAYS]);
		}
		date = Rtc::date;
	}
	
	if(++dvcIndx == DEVC_INDX_PUMP+1) dvcIndx = 0;
	devcs[dvcIndx]->update();
	
	IOHandle::update();
}


//EOF
