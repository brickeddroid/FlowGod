#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "Arduino.h"


#define DHT_LIB_VERSION "0.1.21"

#define DHTLIB_OK               0
#define DHTLIB_ERROR_CHECKSUM   1
#define DHTLIB_ERROR_TIMEOUT    2
#define DHTLIB_ERROR_CONNECT    3
#define DHTLIB_ERROR_ACK_L     	4
#define DHTLIB_ERROR_ACK_H     	5

#define DHTLIB_DHT_WAKEUP       1

#define LOG_INITIALIZED			1

#define PLED_PIN			8

#define SENS0				1
#define SENS1				2
#define SENS2				4
#define SENS_MIX_01			3
#define SENS_MIX_02			5
#define SENS_MIX_12			6
#define SENS_MIX_ALL		7

#define SENS0_AV			1
#define SENS1_AV			2
#define SENS2_AV			4

//#define DHTLIB_DHT_LEADING_ZEROS    6

// max timeout is 100 usec.
// For a 16 Mhz proc 100 usec is 1600 clock cycles
// loops using DHTLIB_TIMEOUT use at least 4 clock cycli
// so 100 us takes max 400 loops
// so by dividing F_CPU by 40000 we "fail" as fast as possible
#ifndef F_CPU
#define DHTLIB_TIMEOUT 1000  // ahould be approx. clock/40000
#else
#define DHTLIB_TIMEOUT (F_CPU/40000)
#endif



struct Log
{
    uint16_t min:10;
    uint16_t avrg:10;
    uint16_t max:10;
    
    void init(uint16_t input);
    void update(uint16_t input);
    uint16_t calcAvrg(uint16_t accumulator, uint16_t new_value, unsigned long nSamples);
    
    uint8_t avrgCnt;
    uint16_t tmpAvrg:10;
    
    Log();
};

struct Reading{
    uint32_t temperature:10;
    uint32_t humidity:10;
    uint32_t state:3;
    uint32_t pled:1;
    
    Reading() : temperature(0), humidity(0), state(0), pled(0) {}
};

class Sensor
{
  public:
    //static bool available(uint8_t mode);
    //static Sensor *regHandles[3];
    
    Sensor(uint8_t dhtPin=7);
    void update();
    
    //void init();
    
    Reading reading;
    
    //uint16_t tempOut:10;
    //uint16_t humiOut:10;
    
    
    const uint8_t id:2;
    uint8_t loginit:1;
    
    Log log[2];
    
    
  private:
    const uint8_t dhtPin;
    
    uint32_t const * millisPtr;
    uint32_t prevMillis;
    uint8_t bits[5];
    
    
    static uint8_t read(Sensor * const sensor);
    
    //static uint8_t avlbl;
    static uint8_t cnt;
    
};


class SensorHandle
{
    public:
		static Sensor *sensors[3];
		
		static uint8_t available(uint8_t mode);
		static void get(uint8_t &mode, Reading &r);
		
		static void update();
		
		static inline bool lightOn(){ return pled; }
	
    private:
		SensorHandle();
		static uint8_t pled;
		static uint8_t avlbl;
	
};
#endif
