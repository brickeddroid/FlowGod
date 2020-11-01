#include "messagehandle.hpp"

#include "message433.hpp"

uint8_t MessageHandle::tx_status;
uint8_t MessageHandle::rx_status;
uint8_t MessageHandle::*segptr = NULL;

uint8_t* MessageHandle::ring_buf_tx[TX_RING_BUF_LEN];
uint8_t MessageHandle::ring_buf_tx_meta[TX_RING_BUF_LEN][3];
uint8_t MessageHandle::ring_buf_tx_indx_last;
uint8_t MessageHandle::ring_buf_tx_indx_curr;

uint8_t MessageHandle::currTxBufIndx = 0;
uint8_t MessageHandle::prevTxBufIndx = 0;

uint8_t MessageHandle::update()
{
	if(Transceiver433::available()){
		Packet433 p;
		if(Transceiver433::decode(&p)){
			uint8_t cmd = p.payload[MSG_CMD_BYTE] & MSG_CMD_MASK;
			uint8_t meta = p.payload[MSG_CMD_BYTE] & MSG_CMD_META;					
			
			//TODO analyze message
			
			switch(meta){
				case MSG_CMD_META_BC:
				break;
				case MSG_CMD_META_RESP:
				break;
				case MSG_CMD_META_ACK:
				break;
				case MSG_CMD_META_NACK:
				break;
			}
			switch(cmd){
				case MSG_CMD_GET:
				break;
				case MSG_CMD_SET:
				break;
				case MSG_CMD_PING:
				break;
				case MSG_CMD_EMSTOP:
				break;
			}
		}	
				
	}
	
	if(status == MSG_HANDLE_AVAILABLE && prevTxBufIndx != currTxBufIndx){
		// Open com port
		status = MSG_HANDLE_OPENCOM;
	}
}

uint8_t MessageHandle::mh_proc_msg(Packet433 &msg)
{
}

/* tx_status MessageHandle
 * TX_STAT_AVAIL	1
 * TX_STAT_RDY_NXT	2
 * TX_STAT_RDY_SEND	4
 * TX_STAT_BUF_FULL	8
 * TX_STAT_BUF_EMPT	16
 */

bool MessageHandle::sendCmd(uint8_t rcvrAddr, uint8_t cmd, uint8_t* segptr, uint8_t seglen)
{
	uint8_t status;
	if(tx_status & TX_STAT_BUF_FULL){
		status = TX_STAT_BUF_FULL;
	} else {	
		ring_buf_tx[ring_buf_tx_indx_last] = segptr;
		ring_buf_tx_meta[ring_buf_tx_indx_last][RING_BUF_TX_INDX_RADDR] = rcvrAddr;
		ring_buf_tx_meta[ring_buf_tx_indx_last][RING_BUF_TX_INDX_CMD]   = cmd;
		ring_buf_tx_meta[ring_buf_tx_indx_last++][RING_BUF_TX_INDX_LEN] = seglen;
		tx_status &= ~TX_STAT_BUF_EMPTY;
		if(ring_buf_tx_indx_last == TX_RING_BUF_LEN){
			ring_buf_tx_indx_last = 0;
		}
		status = TX_RING_BUF_LEN - ring_buf_tx_indx_last + ring_buf_tx_indx_curr;
		if(status > TX_RING_BUF_LEN){
			status -= TX_RING_BUF_LEN;
		}
		if(status == 0){
			tx_status |= TX_STAT_BUF_FULL;
		} else if(status == TX_RING_BUF_LEN){
			tx_status |= TX_STAT_BUF_EMPTY;
		}
	}
	return status;
}
