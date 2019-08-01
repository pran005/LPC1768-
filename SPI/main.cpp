#include <lpc17xx.h> 
#include <string.h>
#include "delay.h"
#include "lcd.h"

/****************************************************SPI MASTER ROUTINES***********************************************/
void init_SPI_Master(uint8_t clock)
  {
		LPC_SC -> PCONP |= (1<<8) ; 
		LPC_PINCON -> PINSEL0 |= 0xC0000000 ;  
		LPC_PINCON -> PINSEL1 |= (1<<2)|(1<<3)|(1<<4)|(1<<5)  ;
		LPC_GPIO0  -> FIODIR |= (1<<15)|(1<<16)|(1<<18) ; 																			// SCK,SSEL,MOSI are output
		
	 
		LPC_SPI -> SPCR |= (1<<5); //| (1<<11) ;                                     	// MSB first, CPOL = CPHASE = 0 , 8 bit 
	
		LPC_SPI -> SPCCR = clock ; 																														 // PCLK/Clock
	}
	
	
unsigned short SPI_Communicate(char data)
	{
		//insert a poll for Tx here ; 
		LPC_SPI -> SPDR = data ; 
		while(!(LPC_SPI->SPSR & 0x80)) ; 
		//uint32_t dummy = LPC_SPI->SPSR ;
		if(LPC_SPI ->SPSR & (1<<6))
			{
				lcd("COLLIDE") ; 
				delayms(800) ; 
			
			}
			if(LPC_SPI ->SPSR & (1<<5))
			{
				lcd("ROVR") ; 
				delayms(800) ; 
			
			}
			if(LPC_SPI ->SPSR & (1<<3))
			{
				lcd("SLAVE") ; 
				delayms(800) ; 
			
			}
			if(LPC_SPI ->SPSR & (1<<4))
			{
				lcd("COLLIDE") ; 
				delayms(800) ; 
			}
		return (LPC_SPI -> SPDR & 0xFFFF) ;
	}
	
uint8_t SPI_string (char *string)
{
	char data ; 
	int len = strlen(string);
	cmd(0xC0) ; 
	lcd(len) ; 
		for (int i=0 ; i<7 ;i++)
	{
		data = SPI_Communicate(*string++) ;
		delayms(80) ; 
	}
	data = SPI_Communicate(0x00) ; 
 return data ; 

}
/****************************************************End of Routines***********************************************/

int main()
	
{
	SystemInit() ;
	init_lcd() ; 
	init_SPI_Master(0x08) ; 
	uint16_t data = 0x00 ;
	uint16_t dat1;	
	
	 
	while(1) 
	{
		//dat1 = SPI_Communicate(data++) ;
		//dat1 = SPI_Communicate('P') ;
		//delayms(800) ;
		//dat1 = SPI_Communicate('R') ;
		//dat1 = SPI_Communicate('A') ;
		//delayms(800) ;
		//dat1 = SPI_Communicate('N') ;
		cmd(0x80) ; 
		dat1 = SPI_string((char *)"Pranjal") ;
		cls() ; 
    lcd(dat1) ;
		delayms(800) ;
	}






}
