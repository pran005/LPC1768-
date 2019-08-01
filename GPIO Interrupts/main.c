#include<lpc17xx.h> 

void EINT3_IRQHandler() 
	{
		LPC_GPIOINT -> IO0IntClr = (1<<9) ;  
		LPC_GPIO2 -> FIOPIN ^= (1<<0) ;
		//for(int i = 0 ; i<100000000 ; i++) ; 
			
		
	}
int main()
{
	SystemInit() ; 
	//LPC_SC -> PCONP |= (1<<15) ; 
	LPC_GPIO0 -> FIODIR &= ~(1<<9) ;
	LPC_GPIO2 -> FIODIR |= (1<<0) ; 
	 
	LPC_GPIOINT -> IO0IntEnR = (1<<9) ; 
	LPC_GPIOINT -> IO0IntClr = (1<<9) ; 
	//LPC_SC -> EXTINT = (1<<3) ; 
	NVIC_EnableIRQ(EINT3_IRQn) ; 
	//int a = NVIC_GetPendingIRQ(EINT3_IRQn) ; 
	//NVIC_SetPendingIRQ(EINT3_IRQn) ; 
	
	while(1) {
	//if(a) 	
	//LPC_GPIO1 -> FIOSET = (1<<1) ;  
	}
		
	
	
	//return 0 ; 
}

