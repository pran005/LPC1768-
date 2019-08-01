#include <lpc17xx.h>
#include "delay.h" 
#include "lcd.h" 


void init_UART0()
{
	LPC_SC -> PCONP |= (1<<3) ; 																      //UART0 power/clock control bit 
	LPC_PINCON -> PINSEL0 |= (0x01<<4)|(0x01<<6) ;								   //P0.2 -> TXD0 & P0.3 -> RXD0  

  LPC_UART0 -> LCR = (0x3<<0) | (1<<7) ;												 //Format : 8-bit char | 1 stop bit | No Parity 
  LPC_UART0 -> DLL = 10 ;																			
  LPC_UART0 -> DLM = 0 ; 
  
	LPC_UART0 -> FCR |= (1<<0) | (1<<1) | (1<<2) ; 							//enable FIFO | clr TX_buff | clr_RX_buff
	LPC_UART0 -> FDR |= (14<<4) | (5<<0)	;										 // BAUD = 115131.58 (115200) 									     
	LPC_UART0 -> LCR &= ~(1<<7) ; 														// clear divisor latch access 
	

/****************************BAUDRATE CALCULATION*****************************/ 
/********BAUD = PCLK/(16*(256*DLM + DLL)*(1 + DIVVAL/MULVAL))****************/
/********BAUD = (MULVAL/MULVAL+DIVVAL) * PCLK/(16*((256*DLM)+DLL))**********/
/**************************************************************************/
}

/*********************************UART 0 TRANSMIT ROUTINES*****************************************/

void UART0_Tx(char data)
{
	while(!(LPC_UART0 -> LSR & (1<<5))) {};
    LPC_UART0 -> THR = data ; 
}

void UART0_Tx(unsigned char data)
{
	while(!(LPC_UART0 -> LSR & (1<<5))) {};
    LPC_UART0 -> THR = data ; 
}

void UART0_Tx(char *string)
{
	while(*string) 
		UART0_Tx(*string++) ; 
}

/**************************************END OF TX************************************************/

/*******************************UART 0 RECIEVE ROUTINES*****************************************/

unsigned char UART0_Rx()
{
	while(!(LPC_UART0 -> LSR & (1<<0))){} ;
		return  LPC_UART0 -> RBR ; 
}

void UART0_Rx_str(char *string)
{
	char i=0 , j=0 ; 
	do
	{
		*(string+i) = UART0_Rx() ; 
		j= *(string+i) ; 
		i++ ; 
		
	} while(j!='\r' && i!='n') ; 
			i++ ;
	  *(string+i) = '\0' ;
}

/*************************************END OF RX**********************************************/
 
#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFFSIZE		0x08

volatile uint32_t UART0Status, UART1Status;
volatile uint8_t UART0TxEmpty = 1, UART1TxEmpty = 1;
volatile uint8_t UART0Buffer[BUFFSIZE] ; 
volatile uint32_t UART0Count = 0, UART1Count = 0;

extern "C"
{
	void UART0_IRQHandler() 
	{
		int iir = LPC_UART0 -> IIR & 0x04 ;
		int LSR = LPC_UART0 -> LSR ; 
		UART0Buffer[UART0Count++] = LPC_UART0 -> RBR ;
		if(UART0Count==BUFFSIZE)
			UART0Count = 0 ; 
	}  
}

int main()
{
	SystemInit() ;
  init_lcd() ; 	
	
	/*** init_UART0 ***/
	
	LPC_SC -> PCONP |= (1<<3) ; 																      //UART0 power/clock control bit 
	LPC_PINCON -> PINSEL0 |= (0x01<<4)|(0x01<<6) ;								   //P0.2 -> TXD0 & P0.3 -> RXD0  

  LPC_UART0 -> LCR = (0x3<<0) | (1<<7) ;												 //Format : 8-bit char | 1 stop bit | No Parity 
  LPC_UART0 -> DLL = 10 ;																			
  LPC_UART0 -> DLM = 0 ; 
  
	LPC_UART0 -> FCR |= (1<<0) | (1<<1) | (1<<2) ; 							//enable FIFO | clr TX_buff | clr_RX_buff
	LPC_UART0 -> FDR |= (14<<4) | (5<<0)	;										 // BAUD = 115131.58 (115200) 									     
	LPC_UART0 -> LCR &= ~(1<<7) ; 														// clear divisor latch access

	LPC_UART0 -> IER |= (1<<0) ; 
	NVIC_EnableIRQ(UART0_IRQn) ; 
	
/****************************BAUDRATE CALCULATION*****************************/ 
/********BAUD = PCLK/(16*(256*DLM + DLL)*(1 + DIVVAL/MULVAL))****************/
/********BAUD = (MULVAL/MULVAL+DIVVAL) * PCLK/(16*((256*DLM)+DLL))**********/
/**************************************************************************/
	
 char str[20] ; 

	while(1) 
	{
	   
		cls(); 
		lcd((char*) "WHILE")	; 
		if(UART0Count!=0)
		{
			for(int i=0;i<8;i++)
			{	
				lcd_data(UART0Buffer[i]) ;
			}
		}			
		delayms(1000) ;		
	}
}



