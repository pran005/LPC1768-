#ifndef lcd_h
#define lcd_h

#define cls() cmd(0x01)  
#define nextline() cmd(0xC0) 

 
// 0.8 is E , 0.9 is Rs


void cmd (char command);
void lcd_data(char data) ; 
void lcd(char* string) ;
void init_lcd() ; 
void lcd (int m);
void lcd (unsigned int m) ; 
void lcd(double d) ; 

#endif

