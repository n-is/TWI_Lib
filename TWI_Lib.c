/*
 * TWI_Lib_2.c
 *
 * Created: 5/28/2017 4:24:42 PM
 * Author : nischal
 */

/************************************************************************/
/*                      SAMPLE PROGRAM                                  */
/*	TWI_Init();
 *	uint8_t arr[4] = {'a', 1, 0xFF, 0b00011101};
 *	char name[14] = "Nischal Nepal";
 *
 *	TWI_Send_Start;
 *	TWI_Transmission_Begin(0x3C, 10, 2);
 *	TWI_Transmitt(arr, 4);
 *	TWI_Send_Rep_Start;
 *	TWI_Transmission_Begin(0x3C, 10, 2);
 *	TWI_Transmitt(name, 14);
 *	TWI_Send_Stop;
 */	

/************************************************************************/

#ifndef F_CPU
#define F_CPU			16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TWI_TX_BUF_SIZE			32

static volatile uint8_t tw_tx_buf[TWI_TX_BUF_SIZE];
static volatile uint8_t tw_tx_index;
static volatile uint8_t tw_tx_buf_len;

static volatile tw_ints interrupt;

void TWI_Init(){
	TWSR = 0;
	TWBR = (F_CPU / SCL_CLK - 16) / 2;
}

void TWI_Transmission_Begin(uint8_t address, uint16_t time_limit, uint8_t tries){		// address = (address << 1) | R/W
	do{
		TWDR = address;
		TWI_Send_Transmitt;
		
		while(interrupt != slave_written){
			--time_limit;
			if(!time_limit){
				break;
			}
			_delay_us(1);
		}
	} while(--tries && interrupt != slave_written);
}

void TWI_Transmitt(void *chunk, uint8_t  len){
	if(interrupt == slave_written){
		if(len <= TWI_TX_BUF_SIZE){
			uint8_t c = len;
			uint8_t *data = (uint8_t *)chunk;
			while(c){
				tw_tx_buf[c] = data[c];
SS				--c;
			}
			tw_tx_buf_len = 0;
			tw_tx_index = 0;
			TWDR = tw_tx_buf[tw_tx_index++];
			TWI_Send_Transmitt;
		}
	}
}

ISR(TWI_vect, ISR_NOBLOCK){
	switch (TWSR & 0xF8) {
		
		case START : {
			interrupt = started;
		}
		
		case REP_START : {
			interrupt = started;
		}
		
		case SLA_W_ACK : {
			interrupt = slave_written;
		}
		
		case DATA_W_ACK : {
			if(tw_tx_buf_len > tw_tx_index){
				TWDR = tw_tx_buf[tw_tx_index++];
				TWI_Send_Transmitt;
			}
		}
		
	}
}

