#include "rotaryencoder.hpp"

#include "dynamicvalue.hpp"
#include "../data/data.hpp"

volatile uint8_t RotaryEncoder::flag = 0;

void RotaryEncoder::begin()
{
    pinMode(PIN_RE_INTRA, INPUT_PULLUP); 		// set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
    pinMode(PIN_RE_INTRB, INPUT_PULLUP); 		// set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
    attachInterrupt(0, interruptPinA, RISING); 	// set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
    attachInterrupt(1, interruptPinB, RISING); 	// set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
}

void RotaryEncoder::interruptPinA()
{
	flag = (PIND & 0xC) | (flag & 3); // read all eight pin values then strip away all but pinA and pinB's values
	if (flag == RE_INTR_A) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
		DynamicValue::state |= DV_CHANGED;
		DynamicValue::value -= DynamicValue::stepwidth; //decrement the encoder's position count
		if(DynamicValue::value < DynamicValue::min || DynamicValue::value > DynamicValue::max){
			DynamicValue::value = DynamicValue::max;
		}
		flag = 0;
    //bFlag = 0; //reset flags for the next turn
    //aFlag = 0; //reset flags for the next turn
	}
	else if ((flag & 0xC) == RE_INTR_FLAGA) flag = RE_ENC_FLAGB; //signal that we're expecting pinB to signal the transition to detent from free rotation
}

void RotaryEncoder::interruptPinB()
{
	flag = (PIND & 0xC) | (flag & 3); //read all eight pin values then strip away all but pinA and pinB's values
	if (flag == RE_INTR_B) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
			DynamicValue::state |= DV_CHANGED;
			DynamicValue::value += DynamicValue::stepwidth; //increment the encoder's position count
		if(DynamicValue::value < DynamicValue::min || DynamicValue::value > DynamicValue::max){
			DynamicValue::value = DynamicValue::min;
		}
		flag = 0;
		//bFlag = 0; //reset flags for the next turn
		//aFlag = 0; //reset flags for the next turn
	}
	else if ((flag & 0xC) == RE_INTR_FLAGB) flag = RE_ENC_FLAGA; //signal that we're expecting pinA to signal the transition to detent from free rotation
}
