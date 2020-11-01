#include "memoryhandle.hpp"

#include "data.hpp"
#include "EEPROM.h"

#include "message433.hpp"

uint8_t MemoryHandle::slot = 0;
uint8_t MemoryHandle::prevslot = 255;
/*
uint8_t Data::memarray[MEM_LEN] = { 0 };
uint8_t Data::ioarray[MEM_LEN] = { 0 };

const uint8_t Data::dvcsegm[8] = {
		SEG_ADDR_LGHT,
		SEG_ADDR_LFAN,
		SEG_ADDR_HEAT,
		SEG_ADDR_HUMI,
		SEG_ADDR_PWM0,
		SEG_ADDR_PWM1,
		SEG_ADDR_PWM2,
		SEG_ADDR_PUMP
};
* 
uint16_t MemoryHandle::readUint16_t(uint8_t indx, uint8_t *seg)
{
    return (((uint16_t)seg[indx])<<8 | seg[indx+1]);
}

void MemoryHandle::writeUint16_t(uint16_t value, uint8_t indx, uint8_t *seg)
{
    seg[indx] = value>>8;
    seg[indx+1] = value & 0xFF;
}
*/
void MemoryHandle::update()
{
	uint8_t *ptr = Data::pd_array;
    uint8_t *mptr = Data::pd_comp_array;
    uint16_t memslot = PD_SEG_ADDR_HEAD + (uint16_t)slot * PD_SLOT_LEN;
	for(uint8_t u = 0; u < PD_OVERALL_SIZE; u++){
		if(*mptr != *ptr){
			*mptr = *ptr;
			EEPROM.write(u + memslot, *mptr);
			if(*Data::pd_comp_array != PD_HEAD_WRITTEN){
				EEPROM.write(PD_HEAD_DATA_W + memslot, PD_HEAD_WRITTEN);
				*Data::pd_comp_array = PD_HEAD_WRITTEN;
			}
		}
		++ptr;
		++mptr;
	}
}


void MemoryHandle::begin()
{
	slot = EEPROM.read(0);
	initialize();
}

//set a few start parameters
void MemoryHandle::initialize()
{
	if(slot == prevslot){
		return;
	}
	EEPROM.write(0, slot);
	prevslot = slot;
    uint8_t *ptr = Data::pd_array;
    uint8_t *mptr = Data::pd_comp_array;
    uint16_t memslot = PD_SEG_ADDR_HEAD + (uint16_t)slot * PD_SLOT_LEN;
    *mptr = EEPROM.read(PD_HEAD_DATA_W + memslot);
    if(*mptr == PD_HEAD_WRITTEN){
		for(uint8_t u = 0; u < PD_OVERALL_SIZE; u++){
			*mptr = EEPROM.read(u + memslot);
			*(ptr++) = *(mptr++);
		}
		return;
	}
    
    // Head
    //ioarray[SEG_ADDR_HEAD] = 0;
    
    
    
/* ----------------------------------------------------------------------------------- */
/* Setting default values if no data is written to the EEPROM, i.e. at first start up  */
    
    // Board   
    *Data::pd_comp_array = 0;
    ptr = &Data::pd_array[PD_SEG_ADDR_BRD];
    //writeUint16_t(0, BRD_INDX_NDAYS, ptr);
    //ioarray[BRD_INDX_ADDR] = 0;  
    ptr[PD_BRD_INDX_ESTOP] = 80;
    ptr[PD_BRD_INDX_STATUS] = 0;
    
    // Light
    ptr = &Data::pd_array[PD_SEG_ADDR_LGHT];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    ptr[PD_LGHT_INDX_ADDR]	 = 16;	
    ptr[PD_LGHT_INDX_ONTHR] = 8; 
    ptr[PD_LGHT_INDX_ONTMI] = 0;
    ptr[PD_LGHT_INDX_OFTHR] = 20;
    ptr[PD_LGHT_INDX_OFTMI] = 0;
    
    // Light Cooling
    //ptr = &ioarray[SEG_ADDR_LFAN];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    
    // Heat
    ptr = &Data::pd_array[PD_SEG_ADDR_HEAT];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    ptr[PD_CLIM_INDX_ADDR]  = 8;
    ptr[PD_CLIM_INDX_STPT]  = 80;
    ptr[PD_CLIM_INDX_DRIFT] = 36;
    
    // Humidifier
    ptr = &Data::pd_array[PD_SEG_ADDR_HUMI];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    ptr[PD_CLIM_INDX_ADDR]  = 4;
    ptr[PD_CLIM_INDX_STPT]  = 120;
    ptr[PD_CLIM_INDX_DRIFT] = 36;
    
    // Pwm Devices
    ptr = &Data::pd_array[PD_SEG_ADDR_PWM0];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    ptr[PD_PWM_INDX_CSN]  = 100;
    ptr[PD_PWM_INDX_CSD]  = 255;
    //ptr[PWM_INDX_MIXN] = 0;
    ptr[PD_PWM_INDX_MIXD] = 8;
    ptr[PD_PWM_INDX_TSP]  = 40;
    ptr[PD_PWM_INDX_TKP]  = 50;
    ptr[PD_PWM_INDX_TKI]  = 20;
    ptr[PD_PWM_INDX_TKD]  = 5;
    ptr[PD_PWM_INDX_HSP]  = 100;
    ptr[PD_PWM_INDX_HKP]  = 40;
    ptr[PD_PWM_INDX_HKI]  = 15;
    ptr[PD_PWM_INDX_HKD]  = 3;
    
    
    ptr = &Data::pd_array[PD_SEG_ADDR_PWM1];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    ptr[PD_PWM_INDX_CSN]  = 100;
    ptr[PD_PWM_INDX_CSD]  = 255;
    //ptr[PWM_INDX_MIXN] = 0;
    ptr[PD_PWM_INDX_MIXD] = 8;
    ptr[PD_PWM_INDX_TSP]  = 40;
    ptr[PD_PWM_INDX_TKP]  = 50;
    ptr[PD_PWM_INDX_TKI]  = 20;
    ptr[PD_PWM_INDX_TKD]  = 5;
    ptr[PD_PWM_INDX_HSP]  = 100;
    ptr[PD_PWM_INDX_HKP]  = 40;
    ptr[PD_PWM_INDX_HKI]  = 15;
    ptr[PD_PWM_INDX_HKD]  = 3;
    
    ptr = &Data::pd_array[PD_SEG_ADDR_PWM2];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    ptr[PD_PWM_INDX_CSN]  = 100;
    ptr[PD_PWM_INDX_CSD]  = 255;
    //ptr[PWM_INDX_MIXN] = 0;
    ptr[PD_PWM_INDX_MIXD] = 8;
    ptr[PD_PWM_INDX_TSP]  = 40;
    ptr[PD_PWM_INDX_TKP]  = 50;
    ptr[PD_PWM_INDX_TKI]  = 20;
    ptr[PD_PWM_INDX_TKD]  = 5;
    ptr[PD_PWM_INDX_HSP]  = 100;
    ptr[PD_PWM_INDX_HKP]  = 40;
    ptr[PD_PWM_INDX_HKI]  = 15;
    ptr[PD_PWM_INDX_HKD]  = 3;
    
    // Pump
    ptr = &Data::pd_array[PD_SEG_ADDR_PUMP];
    //writeUint16_t(0, DVC_INDX_LIFEHR, ptr);
    ptr[PD_PUMP_INDX_NPOT] = 10;
    ptr[PD_PUMP_INDX_OFFT] = 5;
    ptr[PD_PUMP_INDX_ONT]	= 10;
    
    // Dvc States
    
    // Display
    ptr = &Data::pd_array[PD_SEG_ADDR_DISP];
    ptr[PD_DISP_INDX_STBM] = 5;
    ptr[PD_DISP_INDX_SWTS] = 3;
    Data::conv16to8((1<<11)-1, &ptr[PD_DISP_INDX_PICS]);
}

