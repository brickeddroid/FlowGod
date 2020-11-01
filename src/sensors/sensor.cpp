#include "sensor.hpp"

#include "../data/helper.hpp"
#include "../data/data.hpp"

Log::Log()
{
    min  = 0;
    avrg = 0;
    max  = 0;
}

void Log::init(uint16_t input)
{
	min = input;
	max = input;
	avrg = input;
}

void Log::update(uint16_t input)
{
    max = max(input, max);
    min = min(min, input);
    
    tmpAvrg = calcAvrg(tmpAvrg, input, 300);
    if(++avrgCnt == 180){
		avrg = calcAvrg(avrg, tmpAvrg, 144);
		avrgCnt = 0;
    }
      
}

uint16_t Log::calcAvrg(uint16_t accumulator, uint16_t new_value, unsigned long nSamples)
{
	double alpha = nSamples/1000.; 
	return (alpha * new_value) + (1.0 - alpha) * accumulator;
}


uint8_t Sensor::cnt = 1;

Sensor::Sensor(uint8_t pin) 
    : dhtPin(pin), id(cnt), loginit(0)
{
    ++cnt;
}

void Sensor::update()
{    
    reading.state = read(this);
    // these bits are always zero, masking them reduces errors.
    bits[0] &= 0x03;
    bits[2] &= 0x83;
    
    
    // TEST CHECKSUM
    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
    if (bits[4] != sum)
    {
		reading.state = DHTLIB_ERROR_CHECKSUM;
    }
    if(reading.state == DHTLIB_OK){
	// CONVERT AND STORE
		reading.humidity = (((uint16_t)bits[0]<<8) | bits[1]);
		reading.temperature = ((((uint16_t)(bits[2] & 0x7F))<<8) | bits[3]);
	
		if(loginit & LOG_INITIALIZED){
			log[0].update(reading.humidity);
			log[1].update(reading.temperature);
		} else {
			log[0].init(reading.humidity);
			log[1].init(reading.temperature);
			loginit = LOG_INITIALIZED;
		}
    }
    //sensData.update();
}

uint8_t Sensor::read(Sensor * const sensor)
{
    // footprint reduction
    uint8_t leadingZeroBits = 34;
    uint8_t dhtPin = sensor->dhtPin;
    //leadingZeroBits = 40 - leadingZeroBits; // reverse counting...
    
    
    // replace digitalRead() with Direct Port Reads.
    // reduces footprint ~100 bytes => portability issue?
    // direct port read is about 3x faster
    uint8_t bit = digitalPinToBitMask(dhtPin);
    uint8_t port = digitalPinToPort(dhtPin);
    volatile uint8_t *PIR = portInputRegister(port);

    // REQUEST SAMPLE
    pinMode(dhtPin, OUTPUT);
    digitalWrite(dhtPin, LOW); // T-be
    delayMicroseconds(/*DHTLIB_DHT_WAKEUP * */ 1000UL);
    digitalWrite(dhtPin, HIGH); // T-go
    pinMode(dhtPin, INPUT);

    uint16_t loopCount = DHTLIB_TIMEOUT * 2;  // 200uSec max
    // while(digitalRead(pin) == HIGH)
    while ((*PIR & bit) != LOW )
    {
        if (--loopCount == 0) return DHTLIB_ERROR_CONNECT;
    }

    // GET ACKNOWLEDGE or TIMEOUT
    loopCount = DHTLIB_TIMEOUT;
    // while(digitalRead(pin) == LOW)
    while ((*PIR & bit) == LOW )  // T-rel
    {
        if (--loopCount == 0) return DHTLIB_ERROR_ACK_L;
    }

    loopCount = DHTLIB_TIMEOUT;
    // while(digitalRead(pin) == HIGH)
    while ((*PIR & bit) != LOW )  // T-reh
    {
        if (--loopCount == 0) return DHTLIB_ERROR_ACK_H;
    }

    loopCount = DHTLIB_TIMEOUT;

    // INIT BUFFERVAR TO RECEIVE DATA
    uint8_t mask = 128;
    uint8_t idx = 0;

    uint8_t data = 0;
    uint8_t state = LOW;
    uint8_t pstate = LOW;
    uint16_t zeroLoop = DHTLIB_TIMEOUT;
    uint16_t delta = 0;

    // READ THE OUTPUT - 40 BITS => 5 BYTES
    for (uint8_t i = 40; i != 0; )
    {
        // WAIT FOR FALLING EDGE
        state = (*PIR & bit);
        if (state == LOW && pstate != LOW)
        {
            if (i > leadingZeroBits) // DHT22 first 6 bits are all zero !!   DHT11 only 1
            {
                zeroLoop = min(zeroLoop, loopCount);
                delta = (DHTLIB_TIMEOUT - zeroLoop)/4;
            }
            else if ( loopCount <= (zeroLoop - delta) ) // long -> one
            {
                data |= mask;
            }
            mask >>= 1;
            if (mask == 0)   // next byte
            {
                mask = 128;
                sensor->bits[idx] = data;
                ++idx;
                data = 0;
            }
            // next bit
            --i;

            // reset timeout flag
            loopCount = DHTLIB_TIMEOUT;
        }
        pstate = state;
        // Check timeout
        if (--loopCount == 0)
        {
            return DHTLIB_ERROR_TIMEOUT;
        }

    }
    pinMode(dhtPin, OUTPUT);
    digitalWrite(dhtPin, HIGH);

    return DHTLIB_OK;
}

Sensor *SensorHandle::sensors[3] = { new Sensor(PIN_SENS0), new Sensor(PIN_SENS1), new Sensor(PIN_SENS2) };
uint8_t SensorHandle::pled = 0;
uint8_t SensorHandle::avlbl = 0;

void SensorHandle::update()
{
    pled = !digitalRead(PIN_PLED); // negative <=> positive inverted    
    static uint8_t cnt = 0;
    if(++cnt == 3) cnt = 0;
    Sensor* const sensor = sensors[cnt];
    sensor->reading.pled = pled;
    sensor->update();
     
    if(sensor->reading.state == DHTLIB_OK){
		avlbl |= 1 << cnt;
    } else {
		avlbl &= ~(1 << cnt);
    }
}

void SensorHandle::get(uint8_t &mode, Reading &r)
{
    r.pled = pled;
    mode = available(mode);
    switch(mode){
	case SENS0:
	r.temperature = sensors[0]->reading.temperature;
	r.humidity    =	sensors[0]->reading.humidity;
	break;
	case SENS1:
	r.temperature = sensors[1]->reading.temperature;
	r.humidity    =	sensors[1]->reading.humidity;
	break;
	case SENS2:
	r.temperature = sensors[2]->reading.temperature;
	r.humidity    =	sensors[2]->reading.humidity;
	break;
	case SENS_MIX_01:
	r.temperature = ((uint16_t)(sensors[0]->reading.temperature) + sensors[1]->reading.temperature) >> 1;
	r.humidity = ((uint16_t)(sensors[0]->reading.humidity) + sensors[1]->reading.humidity) >> 1;
	break;
	case SENS_MIX_02:
	r.temperature = ((uint16_t)(sensors[0]->reading.temperature) + sensors[2]->reading.temperature) >> 1;
	r.humidity = ((uint16_t)(sensors[0]->reading.humidity) + sensors[2]->reading.humidity) >> 1;
	break;
	case SENS_MIX_12:
	r.temperature = ((uint16_t)(sensors[1]->reading.temperature) + sensors[2]->reading.temperature) >> 1;
	r.humidity = ((uint16_t)(sensors[1]->reading.humidity) + sensors[2]->reading.humidity) >> 1;
	break;
	case SENS_MIX_ALL:
	r.temperature = ((uint16_t)(sensors[0]->reading.temperature) + sensors[1]->reading.temperature + sensors[2]->reading.temperature) / 3;
	r.humidity = ((uint16_t)(sensors[0]->reading.humidity) + sensors[1]->reading.humidity + sensors[2]->reading.humidity) / 3;
	break;
    }
}

uint8_t SensorHandle::available(uint8_t mode)
{
    return (avlbl & mode) ? avlbl & mode : avlbl;
}


