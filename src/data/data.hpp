#ifndef DATA_HPP
#define DATA_HPP

#include "Arduino.h"

#define SEC_MS(sec) ( sec * 1000 )
#define MIN_MS(min)	( min * SEC_MS(60) )
#define HOUR_MS(hr) ( hr  * MIN_MS(60) )

// pin defines
#define PIN_RE_INTRA	2
#define PIN_RE_INTRB	3
#define PIN_BTN			4

#define PIN_SENS0		5
#define PIN_SENS1		6
#define PIN_SENS2		7
#define PIN_PLED		8

#define PIN_PWM0		9
#define PIN_PWM1		10
#define PIN_PWM2		11
#define PIN_LCOOL		12

#define PIN_RCSWITCH	13

#define PIN_RTC_SDA		A0
#define PIN_RTC_SCL		A1

#define PIN_PUMP		A3


//----------PERSISTENT DATA-------------------------
// segmentation defines
#define PD_SEG_LEN_HEAD		1
#define PD_SEG_LEN_BRD		5
#define PD_SEG_LEN_LGHT		7
#define PD_SEG_LEN_LFAN		2
#define PD_SEG_LEN_CLIM		6
#define PD_SEG_LEN_PWM		15
#define PD_SEG_LEN_PUMP		5
#define PD_SEG_LEN_DISP		4

#define PD_SEG_ADDR_HEAD	1
#define PD_SEG_ADDR_BRD		PD_SEG_ADDR_HEAD + PD_SEG_LEN_HEAD
#define PD_SEG_ADDR_LGHT	PD_SEG_ADDR_BRD  + PD_SEG_LEN_BRD
#define PD_SEG_ADDR_LFAN	PD_SEG_ADDR_LGHT + PD_SEG_LEN_LGHT
#define PD_SEG_ADDR_HEAT	PD_SEG_ADDR_LFAN + PD_SEG_LEN_LFAN
#define PD_SEG_ADDR_HUMI	PD_SEG_ADDR_HEAT + PD_SEG_LEN_CLIM
#define PD_SEG_ADDR_PWM0	PD_SEG_ADDR_HUMI + PD_SEG_LEN_CLIM
#define PD_SEG_ADDR_PWM1	PD_SEG_ADDR_PWM0 + PD_SEG_LEN_PWM
#define PD_SEG_ADDR_PWM2	PD_SEG_ADDR_PWM1 + PD_SEG_LEN_PWM
#define PD_SEG_ADDR_PUMP	PD_SEG_ADDR_PWM2 + PD_SEG_LEN_PWM
#define PD_SEG_ADDR_DISP	PD_SEG_ADDR_PUMP + PD_SEG_LEN_PUMP

#define PD_OVERALL_SIZE		PD_SEG_ADDR_DISP + PD_SEG_LEN_DISP

#define PD_SLOT_LEN		100

//header
#define PD_HEAD_DATA_W		0
#define PD_HEAD_WRITTEN	170		// 10101010

//board
#define PD_BRD_INDX_NDAYS	0
#define PD_BRD_INDX_ADDR	2
#define PD_BRD_INDX_ESTOP	3
#define PD_BRD_INDX_STATUS	4

//----------devices----------
#define PD_DVC_INDX_LIFEHR	0

//light
#define PD_LGHT_INDX_ADDR	2
#define PD_LGHT_INDX_ONTHR	3
#define PD_LGHT_INDX_ONTMI	4
#define PD_LGHT_INDX_OFTHR	5
#define PD_LGHT_INDX_OFTMI	6

//lightcooling

//climate
#define PD_CLIM_INDX_ADDR	2
#define PD_CLIM_INDX_STPT	3
#define PD_CLIM_INDX_DRIFT	4
#define PD_CLIM_INDX_SMIX	5

//pwm
#define PD_PWM_INDX_CSN		2
#define PD_PWM_INDX_CSD		3
#define PD_PWM_INDX_MIXN	4
#define PD_PWM_INDX_MIXD	5
#define PD_PWM_INDX_SMIX	6

#define PD_PWM_INDX_TSP		7
#define PD_PWM_INDX_TKP		8
#define PD_PWM_INDX_TKI		9
#define PD_PWM_INDX_TKD		10

#define PD_PWM_INDX_HSP		11
#define PD_PWM_INDX_HKP		12
#define PD_PWM_INDX_HKI		13
#define PD_PWM_INDX_HKD		14

//pump
#define PD_PUMP_INDX_NPOT	2
#define PD_PUMP_INDX_OFFT	3
#define PD_PUMP_INDX_ONT	4

//---------------devices end----------------

//display
#define PD_DISP_INDX_STBM		0
#define PD_DISP_INDX_SWTS		1
#define PD_DISP_INDX_PICS		2

//devices
#define DEVC_INDX_LGHT		0
#define DEVC_INDX_LCOOL		1
#define DEVC_INDX_HEAT		2
#define DEVC_INDX_HUMI		3
#define DEVC_INDX_PWM0		4
#define DEVC_INDX_PWM1		5
#define DEVC_INDX_PWM2		6
#define DEVC_INDX_PUMP		7


//----------VOLATILE DATA-------------------------
/*
RTC => 8Bytes
    2B:Daycount
    3B:Time
    3B:Date
Sensor1 => 16Bytes
    2B:Temp
    6B:TempPeak/Avrg
    2B:Humi
    6B:HumiPeak/Avrg
Sensor2 => 16Bytes
    2B:Temp 
    6B:TempPeak/Avrg
    2B:Humi
    6B:HumiPeak/Avrg
Sensor3 => 16Bytes
    2B:Temp
    6B:TempPeak/Avrg
    2B:Humi
    6B:HumiPeak/Avrg
PWMDevcs => 3Bytes
    1B:Speed PWM1
    1B:Speed PWM2
    1B:Speed PWM3
Status => 8Bytes
    1B:Sensor(incl. Photoled)
    8B:Devices

Summary => 68Bytes
*/


#define VD_SEG_LEN_TIME    8
#define VD_SEG_LEN_SENS    16
#define VD_SEG_LEN_PWM     3
#define VD_SEG_LEN_STATUS  9

#define VD_SEG_ADDR_TIME    0
#define VD_SEG_ADDR_SENS0   VD_SEG_ADDR_TIME +  VD_SEG_LEN_TIME
#define VD_SEG_ADDR_SENS1   VD_SEG_ADDR_SENS0 + VD_SEG_LEN_SENS
#define VD_SEG_ADDR_SENS2   VD_SEG_ADDR_SENS1 + VD_SEG_LEN_SENS
#define VD_SEG_ADDR_PWM     VD_SEG_ADDR_SENS2 + VD_SEG_LEN_SENS
#define VD_SEG_ADDR_STATUS  VD_SEG_ADDR_PWM   + VD_SEG_LEN_PWM

#define VD_OVERALL_SIZE     VD_SEG_ADDR_STATUS + VD_SEG_LEN_STATUS
//Time Seg
#define VD_TIME_INDX_DAYS_UI16   0
#define VD_TIME_INDX_HOUR        2
#define VD_TIME_INDX_MINUTE      3
#define VD_TIME_INDX_SECOND      4
#define VD_TIME_INDX_DATE        5
#define VD_TIME_INDX_MONTH       6
#define VD_TIME_INDX_YEAR        7
//Sens Seg
#define VD_SENS_INDX_TEMP_UI16   0
#define VD_SENS_INDX_TMIN_UI16   2
#define VD_SENS_INDX_TAVG_UI16   4
#define VD_SENS_INDX_TMAX_UI16   6
#define VD_SENS_INDX_HUMI_UI16   8
#define VD_SENS_INDX_HMIN_UI16   10
#define VD_SENS_INDX_HAVG_UI16   12
#define VD_SENS_INDX_HMAX_UI16   14
//Pwm Seg
#define VD_PWM0_INDX_CSPEED      0
#define VD_PWM1_INDX_CSPEED      1
#define VD_PWM2_INDX_CSPEED      2
//Status Seg
#define VD_STATUS_INDX_SENSOR    0
#define VD_STATUS_INDX_DEVC0     1
#define VD_STATUS_INDX_DEVC1     2
#define VD_STATUS_INDX_DEVC2     3
#define VD_STATUS_INDX_DEVC3     4
#define VD_STATUS_INDX_DEVC4     5
#define VD_STATUS_INDX_DEVC5     6
#define VD_STATUS_INDX_DEVC6     7
#define VD_STATUS_INDX_DEVC7     8

struct Data {
	static uint8_t pd_array[PD_OVERALL_SIZE];
	static uint8_t pd_comp_array[PD_OVERALL_SIZE];
	static const uint8_t pd_dvc_seg[8];
	
	static uint8_t vd_array[VD_OVERALL_SIZE];
	
	static uint16_t conv8to16(uint8_t *ptr);
	
	static void conv16to8(uint16_t ui16, uint8_t *ptr);
};


#endif
