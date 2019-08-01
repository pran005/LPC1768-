#include <lpc17xx.h>
#include "delay.h" 
#include "lcd.h" 

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
	
/****************************BAUDRATE CALCULATION*****************************/ 
/********BAUD = PCLK/(16*(256*DLM + DLL)*(1 + DIVVAL/MULVAL))****************/
/********BAUD = (MULVAL/MULVAL+DIVVAL) * PCLK/(16*((256*DLM)+DLL))**********/
/**************************************************************************/
	
 char str[20] ; 

	while(1) 
	{
		UART0_Tx((char *)"Hello UART\r\n") ;
		//char data = UART0_Rx() ; 
		//lcd_data(data) ;
		UART0_Rx_str(str) ; 
		lcd(str)  ; 
	  delayms(800) ; 		
	}
}

