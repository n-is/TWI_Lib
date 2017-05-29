/*
 * TWI_Lib_1.h
 *
 * Created: 5/28/2017 4:25:20 PM
 *  Author: nischal
 */ 


#ifndef TWI_LIB_H_
#define TWI_LIB_H_


#define SCL_CLK			100000L

#define TWI_SUCCESS		0xFF
#define TWI_NO_DATA		0xF8

enum {
	START		=	0x08,
	REP_START	=	0x10,
	
	SLA_W_ACK	=	0x18,
	SLA_W_NACK	=	0x20,
	DATA_W_ACK	=	0x28,
	DATA_W_NACK	=	0x30,
	
	DATA_R_ACK,
	DATA_R_NACK
	};

typedef enum {
	started,
	slave_written,
	} tw_ints;
	
	
#define TWI_Send_Start		( TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA) ); while(interrupt != started)
#define TWI_Send_Stop		( TWCR = (1 << TWEN)  | (1 << TWIE) | (1 << TWSTO)				 ); while(TWCR & (1 << TWSTO))
#define TWI_Send_Rep_Start	( TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA) ); while(interrupt != started)
#define TWI_Send_Transmitt	( TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE)				 )
#define TWI_Send_ACK		( TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA)	 )
#define TWI_Send_NACK		( TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE)				 )




#endif /* TWI_LIB_H_ */
