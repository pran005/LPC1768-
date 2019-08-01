#include <lpc17xx.h> 
#include "delay.h"

void delayms(uint32_t ms)
{
	LPC_SC -> PCONP |= (1<<1) ;
  
    LPC_TIM0 -> PR = 24999 ; 																		//25000 , indexing starts from 0 	
																											// 25*10^6*10^-3
	LPC_TIM0 -> TCR = (1<<1) ;  

    LPC_TIM0 -> TCR = (1<<0) ; 
	
	while(LPC_TIM0->TC < ms) ; 
	
	LPC_TIM0 -> TCR = 0x00 ; 

}	
