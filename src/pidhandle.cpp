#include "pidhandle.hpp"

#include "helper.hpp"

//uint16_t const * PidHandle::input = 0;

PidHandle::PidHandle()
    : state(0), lInput(0)
{
}

void PidHandle::initialize(uint16_t input)
{
	outputSum = 0;
	lInput = input;
	if(outputSum > PID_OUTMAX) outputSum = PID_OUTMAX;
	else if(outputSum < PID_OUTMIN) outputSum = PID_OUTMIN;
}

bool PidHandle::calculate(uint16_t input, uint8_t *params)
{
    //log[isLightOn].update(input);
    int8_t kp = (~(params[1]))+1;
    int8_t ki = (~((params[2]) << 1))+1;
    int8_t kd = (~((params[3]) >> 1))+1;
    
    
    if((state & PID_ACTIVE) == 0) return false;
    //uint8_t offset = isLightOn*3;
    /*Compute all the working error variables*/
    int16_t error = convertEncU8toU16(params[0]) - input;
    outputSum += ki * error;
    
    if(outputSum > PID_OUTMAX) outputSum = PID_OUTMAX;
    else if(outputSum < PID_OUTMIN) outputSum = PID_OUTMIN;
    int16_t dInput = input - lInput;
    /*Compute PID Output*/
    int32_t tmpOutput = kp * error + outputSum - kd * dInput;
    
    if(tmpOutput > PID_OUTMAX) tmpOutput = PID_OUTMAX;
    else if(tmpOutput < PID_OUTMIN) tmpOutput = PID_OUTMIN;
    output = tmpOutput>>8;
	    
    /*Remember some variables for next time*/
    lInput = input;
    
    return true;
}

void PidHandle::enableAutoMode(bool enabled, uint16_t input)
{ 
  if(enabled && (state & PID_ACTIVE) == 0) {
    initialize(input);
  }  
  state ^= PID_ACTIVE; 
}
/*
uint8_t PidHandle::getParam(uint8_t indx)
{
    int8_t val;
    //PidData* pid;
    //uint8_t kistate;
    //if(indx < 6){
	//pid = &pid_humi;
    //}
    //else{
	//pid = &pid_temp;
	//indx -= 6;
    //}
    
    //if(indx == 2){
	//kistate = pid->state & PID_KIFLAGN;
    //}
    //else if(indx == 5){
	//kistate = pid->state & PID_KIFLAGD;
    //}
    
    //Inverse
    val = ~(params[indx]-1);
    switch(indx)
    {
	case 0:
	return val;
	case 1:
	return (val >> 1);
	break;
	case 2:
	if(state & PID_KIFLAG){
	    return (val<<1) | 1;
	} 
	return (val << 1);
	break;
	default: 
	break;
    } 
}

void PidHandle::setParam(int8_t val, uint8_t indx)
{
    if(val < 0) return;
    
    //PidData* pid;
    //if(indx < 6){
	//pid = &pid_humi;
    //} 
    //else{
	//pid = &pid_temp;
	//indx -= 6;
    //}
    if(indx == 1){
	val <<= 1;
    }
    else if(indx == 2){
	if(val&1){
	    state |= PID_KIFLAG;      
	}
	else{
	    state &= ~(PID_KIFLAG);
	}
	val >>= 1;      
    }
    
    //Inverse
    val = (~val)+1;
    params[indx] = val;
}
*/
