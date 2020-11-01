#ifndef MESSAGE_TEST_HPP
#define MESSAGE_TEST_HPP

#include "Arduino.h"

struct Message {
	uint8_t rcvrAddr;
	uint8_t cmd;
	uint8_t segment;
	uint8_t dataLen;
};

#define PCK_ADDR_RCVR	0
#define PCK_ADDR_SEND	1
#define PCK_PAYL_DATA0	2
#define PCK_PAYL_DATA1	3
#define PCK_PAYL_DATA2	4
#define PCK_PAYL_DATA3	5
#define PCK_PAYL_DATA4	6
#define PCK_PAYL_DATA5	7
#define PCK_PAYL_DATA6	8
#define PCK_PAYL_DATA7	9
#define PCK_PAYL_DATA8	10
#define PCK_PAYL_DATA9	11



/*
 * PAYLOAD:
 * 		BYTE0 => CMD
 * 		BYTE1-9 => depends on CMD
 * 		
 * CMD:
 * 	PING:
 * 		IGNORE
 * 	BC:
 * 		IGNORE
 * 	GET/SET:
 * 		BYTE1 => SEGMENT
 * 		BYTE2 => DATA_LEN
 * 		BYTE3 => CURRENT_POSITION => if CURRENT_POSITION == DATA_LEN => cmd succeed   
 */
 
#define MSG_CMD_BC		0
#define MSG_CMD_PING	1
#define MSG_CMD_GET		2
#define MSG_CMD_SET		3

#define PCK_CMD			PCK_PAYL_DATA0
#define PCK_CMD_SEG		PCK_PAYL_DATA1
#define PCK_CMD_DLEN	PCK_PAYL_DATA2
#define PCK_CMD_CPOS	PCK_PAYL_DATA2

struct Packet {
	uint8_t packet[12];
};

#define TRM_BUF_LEN		5

struct Transmitter {
	
	bool queue(Message msg);
	
	bool available();
	bool interpretPacket();
	
private:
	uint8_t wHead;
	uint8_t rHead; 

	Message buffer[TRM_BUF_LEN];
	
	bool send();
};

#endif
