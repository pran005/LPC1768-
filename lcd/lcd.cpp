#include <lpc17xx.h> 
#include "delay.h"
#include <string.h>
#include "lcd.h" 
//#include <stdio.h> 


// 0.8 is E , 0.9 is Rs

void cmd (char command)
{
	LPC_GPIO0 -> FIOMASK0 = 0x0f ;
	LPC_GPIO0 -> FIOCLR |= (1<<9) ; 
	LPC_GPIO0 -> FIOSET |= (1<<8) ; 
	LPC_GPIO0 -> FIOPIN0 = command ;																//cmd & 0xf0 
	delayms(2) ; 
  LPC_GPIO0 -> FIOCLR = (1<<8) ;  
  
	LPC_GPIO0 -> FIOSET |= (1<<8) ;
  LPC_GPIO0 -> FIOMASK1 = 0x0f ;	
  LPC_GPIO0 -> FIOPIN0 = command<<4 ;  	
	LPC_GPIO0 -> FIOMASK = 0x00000000 ; 
	delayms(2) ; 
	LPC_GPIO0 -> FIOCLR |= (1<<8) ; 
}
	
void lcd_data(char data)
{
	LPC_GPIO0 -> FIOMASK0 = 0x0f ;
	LPC_GPIO0 -> FIOSET |= (1<<8)|(1<<9) ; 
	 
	LPC_GPIO0 -> FIOPIN0 = data & 0xf0 ; 
	delayms(2) ; 
  LPC_GPIO0 -> FIOCLR = (1<<8) ;  
  
	LPC_GPIO0 -> FIOSET |= (1<<8) ;
  LPC_GPIO0 -> FIOMASK1 = 0x0f ;	
  LPC_GPIO0 -> FIOPIN0 = data<<4 ;  	
	LPC_GPIO0 -> FIOMASK = 0x00000000 ; 
	delayms(2) ; 
	LPC_GPIO0 -> FIOCLR |= (1<<8) ; 
}

void lcd(char* string) 
{
	while(*string)
	lcd_data(*string++) ; 	
}
void init_lcd()
{
	
	LPC_GPIO0 -> FIODIR |= (1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9) ;
	 
	cmd(0x02) ; 
	cmd(0x28) ; 
	cmd(0x0E) ; 
	cmd(0x06) ; 
	cmd(0x01) ; 
	cmd(0x80) ;
  
	delayms(10) ; 

}
void lcd (int m)
{
	uint8_t i=0, flag=0,r,p  ;
	char str[100] ;	
	if(m==0)
	{ str[i] = '0' ;
		i++ ;
		str[i]= '\0' ;
	}
	if(m<0)
	{ m=-m ;
		flag=1 ;
	}
	while(m!=0)
	{
		r=m%10 ;
		str[i++] = r+ '0' ;
		m=m/10 ;
		 
	}
	if(flag==1)
	str[i++] = '-' ;
	str[i++] = '\0' ;
	p=strlen(str) ; 
	for(int i=p-1 ; i>=0 ; i--)
	lcd_data(str[i]) ;

}
void lcd (unsigned int m)
{
	uint8_t i=0, r,p  ;
	char str[100] ;	
	if(m==0)
	{ str[i] = '0' ;
		i++ ;
		str[i]= '\0' ;
	}
	while(m!=0)
	{
		r=m%10 ;
		str[i++] = r+ '0' ;
		m=m/10 ;
		 
	}
	str[i++] = '\0' ;
	p=strlen(str) ; 
	for(int i=p-1 ; i>=0 ; i--)
	lcd_data(str[i]) ;

}
void lcd(double d)
{
	int i = int(d) ; 
	float f = d-i ; 
	lcd(i) ; 
  lcd_data('.') ;
  if(d>0)	
  lcd((int)(f*100000)) ; 
  else if(d<0) 
 	lcd((int)(-f*100000)) ;	
	/*char str[20] ; 
	memset(str,0,20) ; 
	sprintf(str,"%f",d) ; 
	lcd(str) ;
  */	
}
