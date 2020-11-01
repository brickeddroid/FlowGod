#include "remotecontrol.hpp"


uint8_t RemoteControl::nRepeat = 10;
int16_t RemoteControl::pulseLength = 350;
// Replaced by LED_PIN due a fixed pin layout, now rc-pin is always dpin13
// is the register representation of pin PIN_RCSWITCH(13)
#define LED_PIN	32

uint8_t const * RemoteControl::sysAddrPtr = NULL;

void RemoteControl::begin(uint8_t const * const _sysAddrPtr)
{
  sysAddrPtr = _sysAddrPtr;  
  DDRB |= LED_PIN;
}

void RemoteControl::switchRcDevc(uint8_t address, bool bOn){
	noInterrupts();
	uint16_t binSwitches = 0;
	uint8_t k = 11;
	int8_t u;
	binSwitches = (((uint16_t)(*sysAddrPtr)&31)<<7) | ((address & 31)<<2) | (1 << bOn); 
	
	for(uint8_t v = 0; v < nRepeat; v++){
		for( u = 11; u >= 0; u--){
			transmit(1,3);
			if((binSwitches >> u) & 1) {
				sendT0();
			}
			else{
				sendT1();
			}
		}
		transmit(1,31);
	}
	interrupts();
}

void RemoteControl::transmit(int nHighPulses, int nLowPulses) {
	PORTB |= LED_PIN;
	delayMicroseconds( pulseLength * nHighPulses);
	PORTB &= ~LED_PIN;
	delayMicroseconds( pulseLength * nLowPulses);
}


/**
 * Sends a Tri-State "0" Bit
 *            _     _
 * Waveform: | |___| |___
 */
void RemoteControl::sendT0() {
  //transmit(1,3);
  transmit(1,3);
}
/**
 * Sends a Tri-State "F" Bit
 *            _     ___
 * Waveform: | |___|   |_
 */
void RemoteControl::sendT1() {
  //transmit(1,3);
  transmit(3,1);
}

void RemoteControl::switchOff(uint8_t address)
{
  switchRcDevc(address, false);
}

void RemoteControl::switchOn(uint8_t address)
{
  switchRcDevc(address, true);
}
