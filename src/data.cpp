/*generated file data.cpp*/
#include "data.hpp"

uint8_t Data::pd_array[PD_OVERALL_SIZE] = { 0 };
uint8_t Data::pd_comp_array[PD_OVERALL_SIZE] = { 0 };
const uint8_t Data::pd_dvc_seg[8] = {
	PD_SEG_ADDR_LGHT,
	PD_SEG_ADDR_LFAN,
	PD_SEG_ADDR_HEAT,
	PD_SEG_ADDR_HUMI,
	PD_SEG_ADDR_PWM0,
	PD_SEG_ADDR_PWM1,
	PD_SEG_ADDR_PWM2,
	PD_SEG_ADDR_PUMP
};

uint8_t Data::vd_array[VD_OVERALL_SIZE] = { 0 };


uint16_t Data::conv8to16(uint8_t *ptr)
{
	uint16_t ui16 = *(ptr++);
	ui16 |= (uint16_t)(*ptr) << 8;
	return ui16;
}

void Data::conv16to8(uint16_t ui16, uint8_t *ptr)
{
	*(ptr++) = (ui16 & 0xF);
	*ptr = (ui16>>8);
}
//EOF DATA_CPP
