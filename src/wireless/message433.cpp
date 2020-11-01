/*generated file message433.cpp*/
#include "message433.hpp"
/*
//ctor
Message433::Message433()
{
}

//dtor
Message433::~Message433()
{
}
*/

static volatile uint8_t  status = 0;
static volatile uint16_t myaddrenc = 0;
static volatile uint16_t bcaddrenc = 0;

//Receiving
//msg rx_msg;
static volatile uint16_t rx_buf[MSG_PLOAD_WCRC_LEN] = { 0 };	
static volatile uint8_t  rx_reading_buf = 0;
static volatile uint8_t  rx_integrator = 0;
static volatile uint16_t rx_bits = 0;
static volatile uint8_t  rx_bit_count = 0;
static volatile uint8_t  rx_buf_indx = 0;

static uint8_t rx_bad = 0;
static uint8_t rx_good = 0;

static volatile uint8_t rx_pll_ramp = 0;

//Transmitting
static volatile uint8_t tx_msg_count;
static volatile uint8_t tx_bit_count;
static volatile uint8_t tx_buf_indx; 
static volatile uint8_t tx_sample;
static volatile uint16_t tx_buf[MSG_MAX_LEN] = {
	0xAAA, 0xAAA, 0xAAA, 0xB38 
};

static const uint8_t symbols[] =
{
    0xd,  0xe,  0x13, 0x15, 0x16, 0x19, 0x1a, 0x1c, 
    0x23, 0x25, 0x26, 0x29, 0x2a, 0x2c, 0x32, 0x34
};

uint16_t encodeToSymbol(uint8_t value)
{
	uint16_t enc = 0;
	enc = symbols[value >> 4];
	enc = ((uint16_t)symbols[value & 0xF])<<6 | (enc & 0x3F);	
	return enc;
}

bool decodeSymbol(uint16_t sym12bit, uint8_t &outDec)
{
	bool lNibbleFound = false;
	bool hNibbleFound = false;
	outDec = 0;
	//Serial.println("SYM:");
	//Serial.println(sym12bit);
	//Serial.println(sym12bit, BIN);
	
	for(uint8_t u = 0; u < 16; u++)
	{
		if(!lNibbleFound && symbols[u] == ((sym12bit >> 6) & 0x3F)){
			lNibbleFound = true;
			outDec = (outDec & 0xF0) | u;
		}
		if(!hNibbleFound && symbols[u] == (sym12bit & 0x3F)) {
			hNibbleFound = true;
			outDec = (outDec & 0xF) | (u << 4);
		}
		if(hNibbleFound && lNibbleFound){
			return true;
		}
	}
	return false;
}

void crc_update (uint16_t& crc, uint8_t data)
{
    data ^= lo8 (crc);
    data ^= data << 4;

    crc = ((((uint16_t)data << 8) | hi8 (crc)) ^ (uint8_t)(data >> 4) 
			^ ((uint16_t)data << 3));
}

void Transceiver433::begin(uint8_t addr)
{
	myaddrenc = encodeToSymbol(addr);
	bcaddrenc = encodeToSymbol(0xFF);
	cli();
	//Serial.println("Transmission setup...");
	DDRB |= (1 << PINB_OUT);
	DDRB = ~(1 << PINB_IN);  
	
	TCCR1B = TCCR1B & 0b11111000 | 0x01;   // Set Timer1 (Pin 9 & 10) to 31kHz, to avoid high frequent noises from fans on pwm pins(9,10,11)
	//TCCR2B = TCCR2B & 0b11111000 | 0x01;   // Set Timer2 (Pin 3 & 11) to 31kHz, to avoid high frequent noises from fans on pwm pins(9,10,11)
	
	//TCCR1A = (1<<WGM01); // CTC Modus
	//TCCR1B |= (1<<CS01); // Prescaler 8
	//OCR1A = 0xFF;
	// Compare Interrupt erlauben
	//TIMSK |= (1<<OCIE0A);
	TIMSK1 |= _BV(OCIE1A);
	
	//TCCR0A = 0;
	//TCCR0B = 0;

	//TCCR0A |=(1<<WGM01); //Start timer 1 in CTC mode Table 11.5
	//TIMSK0 |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
	//OCR0A=127; //CTC Compare value
	//TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler =1024 Table 11.6
	
	status |= MSG_RX_ENABLED;
	//Serial.println(status, BIN);
	sei();
}

uint8_t Transceiver433::rxGood()
{
	return rx_good;
}

uint8_t Transceiver433::rxBad()
{
	return rx_bad;
}

uint8_t Transceiver433::ioStatus()
{
	return status;
}

bool Transceiver433::available()
{	
	return status & MSG_RX_DONE;
}

uint8_t Transceiver433::txMsgCount()
{
	return tx_msg_count;
}

uint8_t Transceiver433::txBufIndx()
{
	return tx_buf_indx;
}

uint8_t Transceiver433::txBitCount()
{
	return tx_bit_count;
}

bool Transceiver433::decode(Packet433 * const m)
{
	uint16_t crc = 0xFFFF;
	status &= ~MSG_RX_DONE;
	uint8_t decValue = 0;
	for(uint8_t u = 0; u < MSG_PLOAD_WCRC_LEN; u++)
	{
		if(decodeSymbol(rx_buf[u], decValue)){
			if(u < MSG_PAYLOAD_LEN){
				m->payload[u] = decValue;
			}
			crc_update(crc, decValue);
		} else {
			++rx_bad;
			return false;
		}
	}
	if(crc == 0xF0B8){
		++rx_good;
		return true;
	}
	++rx_bad;
	return false;
}

void Transceiver433::send(Packet433 const * const m)
{
	uint16_t crc = 0xFFFF;
	const uint8_t *pload = m->payload;
	for(uint8_t u = MSG_HEADER_LEN; u < (MSG_HEADER_LEN + MSG_PAYLOAD_LEN); u++){
		crc_update(crc, *pload);
		tx_buf[u] = encodeToSymbol(*(pload++));
	}
	crc = ~crc;
	tx_buf[MSG_HEADER_LEN+MSG_PAYLOAD_LEN] = encodeToSymbol(crc & 0xFF);
	tx_buf[MSG_HEADER_LEN+MSG_PAYLOAD_LEN+1] = encodeToSymbol(crc >> 8);
	
	status |= MSG_TX_ENABLED;
	tx_sample = 0;
	tx_bit_count = 0;
	tx_buf_indx = 0;
	
	//wait until message has gone...
	while(status & MSG_TX_ENABLED)
	;
}

ISR(TIMER1_COMPA_vect){
	if((status & (MSG_RX_ENABLED | MSG_TX_ENABLED)) == MSG_RX_ENABLED){
		rx_reading_buf <<= 1;
		rx_reading_buf |= ((PINB >> PINB_IN)& 1);
	}
	
	if(status & MSG_TX_ENABLED && tx_sample++ == 0){
		if(tx_buf_indx >= MSG_MAX_LEN){
			status &= ~MSG_TX_ENABLED;
			PORTB &= ~(1 << PINB_OUT);
			++tx_msg_count;
		} else {
			PORTB = (tx_buf[tx_buf_indx] & (1 << tx_bit_count)) ? PORTB | (1 << PINB_OUT) : PORTB & ~(1 << PINB_OUT);
			//PORTB = (tx_buf[tx_buf_indx] & (1 << tx_bit_count)) ? PORTB | (1 << PINB_LED) : PORTB & ~(1 << PINB_LED);
			if(++tx_bit_count >= 12){
				tx_bit_count = 0;
				++tx_buf_indx;
			}
		}
	}
	if(tx_sample > 7){
		tx_sample = 0;
	}
	
	if((status & (MSG_RX_ENABLED | MSG_TX_ENABLED)) == MSG_RX_ENABLED){
		if(rx_reading_buf & 1){
			++rx_integrator;
		}
		if((rx_reading_buf&1) != ((rx_reading_buf>>1)&1)){
			rx_pll_ramp += ((rx_pll_ramp < MSG_RAMP_TRANSITION) 
				? MSG_RAMP_INC_RETARD 
				: MSG_RAMP_INC_ADVANCE);
		} else {
			rx_pll_ramp += MSG_RAMP_INC;
		}
		
		if(rx_pll_ramp >= MSG_RAMP_LEN){
			rx_bits >>= 1;
			if(rx_integrator >= 5){
				rx_bits |= 0x800;
			}
			rx_pll_ramp -= MSG_RAMP_LEN;
			rx_integrator = 0;
			
			if(status & MSG_RX_ACTIVE){
				if(++rx_bit_count >= 12){
					rx_buf[rx_buf_indx++] = rx_bits;
					rx_bit_count = 0;
					if(rx_buf[0] != myaddrenc && rx_buf[0] != bcaddrenc)
						status &= ~MSG_RX_ACTIVE;
					if(rx_buf_indx >= MSG_PLOAD_WCRC_LEN){
						status |= MSG_RX_DONE;
						status &= ~MSG_RX_ACTIVE;						
					} 
				}
			} else if(rx_bits == 0xB38){
				status |= MSG_RX_ACTIVE;
				status &= ~MSG_RX_DONE;
				rx_buf_indx = 0;
				rx_bit_count = 0;
			}
		}
		
	}
}
//EOF
