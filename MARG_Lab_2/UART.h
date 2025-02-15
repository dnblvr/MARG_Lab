
#ifndef _LCD_H_
	#define _LCD_H_
	
	#include <TM4C123GH6PM.h>
	#include <stdio.h>

	void Delay(unsigned long counter);
	void uart5_init(void);
	void UART5_Transmitter(unsigned char data);
	void printstring(char *str);

	
#endif // _LCD_H_