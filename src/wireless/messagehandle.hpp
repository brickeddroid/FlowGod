#ifndef MESSAGEHANDLE_HPP
#define MESSAGEHANDLE_HPP


#define TX_RING_BUF_LEN		8

#define RING_BUF_TX_INDX_RADDR		0
#define RING_BUF_TX_INDX_CMD		1
#define RING_BUF_TX_INDX_LEN		2

#define TX_STAT_AVAIL		1
#define TX_STAT_RDY_NXT		2
#define TX_STAT_RDY_SEND	4
#define TX_STAT_BUF_FULL	8
#define TX_STAT_BUF_EMPTY	16

#define TXE_META_ASC_MASK	0x07
#define TXE_META_DIS_FLAG	0x08
#define TXE_META_ALR_SENT	0x10
#define TXE_META_OPN_CPORT	0x80
		

struct TxElement 
{
	uint8_t *segptr;
	uint8_t meta;
	
	uint8_t rcvrAddr;
	uint8_t cmd;
	uint8_t seglen;	
};

struct MessageHandle {
	static uint8_t status;
	
	static uint8_t tx_status;
	static uint8_t rx_status;
	
	static uint8_t endIndx;
	static uint8_t currIndx;
	static uint8_t *segptr;
	
	static uint8_t *ring_buf_tx[TX_RING_BUF_LEN];
	static uint8_t ring_buf_tx_meta[TX_RING_BUF_LEN][3];
	static uint8_t ring_buf_tx_indx_last;
	static uint8_t ring_buf_tx_indx_curr;
	
	static uint8_t update();
	
	static uint8_t mh_proc_msg(Packet433 &packet);
	
	static bool sendCmd(uint8_t rcvrAddr, uint8_t cmd, uint8_t* segptr, uint8_t seglen);
	
	static uint8_t enqueueToTxBuffer(TxElement elem);		
};
#endif
