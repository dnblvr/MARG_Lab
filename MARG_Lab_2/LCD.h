
#ifndef _LCD_H_
	#define _LCD_H_


	#include <TM4C123GH6PM.h>
	#include <stdio.h>
	
	
	void LCD_nibble_write(char data, unsigned char control);
	void delay_ms(int n);
	void PORTS_init();
	void LCD_init();
	void LCD_command(unsigned char command);
	void LCD_data(char data);
	
#endif // _LCD_H_