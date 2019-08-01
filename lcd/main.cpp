#include <lpc17xx.h> 
#include "delay.h"
#include "lcd.h" 
 

int main()
{
	SystemInit() ; 
	init_lcd() ; 
	int i = -308 ;
  float a = (float)i/12 ; 
	while(1)
	{	

	lcd((char*)"PRANJAL") ; 
	lcd(a) ; 
	delayms(1000) ; 
	cmd(0xC0) ; 
  lcd((char*)"SHRIVASTAVA ") ;
  lcd(i) ; 		
  delayms(1000) ; 
  cmd(0x01) ;
   		
  } 

}
