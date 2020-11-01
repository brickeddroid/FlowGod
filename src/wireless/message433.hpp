/*generated file message433.hpp*/
#ifndef MESSAGE433_HPP
#define MESSAGE433_HPP

#include "Arduino.h"

#define MSG_MAX_LEN			19

#define MSG_HEADER_LEN		4
#define MSG_PAYLOAD_LEN		13
#define MSG_CRC_LEN			2

#define MSG_PLOAD_WCRC_LEN	15

#define MSG_TX_ENABLED	1 << 0
#define MSG_TX_DONE		1 << 1
#define MSG_RX_ENABLED	1 << 4
#define MSG_RX_ACTIVE	1 << 5
#define MSG_RX_DONE		1 << 6

#define MSG_ADDR_RCVR	0
#define MSG_ADDR_SNDR	1
#define MSG_CMD_BYTE	2
#define MSG_INFO_BYTE	3
#define MSG_MEM_ADDR	4
#define MSG_DATA_BYTE0	5
#define MSG_DATA_BYTE1	6
#define MSG_DATA_BYTE2	7
#define MSG_DATA_BYTE3	8
#define MSG_DATA_BYTE4	9
#define MSG_DATA_BYTE5	10
#define MSG_DATA_BYTE6	11
#define MSG_DATA_BYTE7	12

#define MSG_CMD_MASK		0x0F
#define MSG_CMD_META		0xF0

#define MSG_CMD_META_BC		0x10
#define MSG_CMD_META_RESP	0x20
#define MSG_CMD_META_ACK	0x40
#define MSG_CMD_META_NACK	0x80

#define MSG_CMD_GET			0x1
#define MSG_CMD_SET			0x2
#define MSG_CMD_PING		0x3
#define MSG_CMD_EMSTOP		0x4

#define MSG_INFO_MASK_DATA	0x07
#define MSG_INFO_MASK_DTYPE	0x18
#define MSG_INFO_MASK_SEGM	0xE0

#define PINB_IN   	4
#define PINB_OUT  	5
#define PINB_LED	5

#define MSG_SAMPLES_PER_BIT 	8
#define MSG_RAMP_LEN	 		160
#define MSG_RAMP_INC 			(MSG_RAMP_LEN/MSG_SAMPLES_PER_BIT)
#define MSG_RAMP_TRANSITION 	MSG_RAMP_LEN/2
#define MSG_RAMP_ADJUST 		9
#define MSG_RAMP_INC_RETARD		(MSG_RAMP_INC-MSG_RAMP_ADJUST)
#define MSG_RAMP_INC_ADVANCE    (MSG_RAMP_INC+MSG_RAMP_ADJUST)

#define lo8(x) ((x)&0xFF)
#define hi8(x) ((x)>>8)


struct Packet433 {		
	uint8_t payload[MSG_PAYLOAD_LEN];
};

struct Transceiver433 {
	static uint8_t ioStatus();
	static uint8_t txMsgCount();
	static uint8_t txBufIndx();
	static uint8_t txBitCount();
		
	static uint8_t rxBad();
	static uint8_t rxGood();
	
	static void begin(uint8_t addr);
	static bool available();
	static bool decode(Packet433* const m);
	static void send(Packet433 const * const m);	
};

#endif //MESSAGE433_HPP
