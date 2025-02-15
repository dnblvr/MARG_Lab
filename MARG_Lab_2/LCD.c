
#include "LCD.h"


/* LCD nibble-write ------------------------------------------------------ */
void LCD_nibble_write(char data, unsigned char control) {
	/*
	 *	This is used to write a function to write 4-bit data (one nibble) at a time to LCD. 
	 *	For 8-bit write data/command, we will use this function twice. This function needs
	 *	two arguments:
	 *			the value 	to write, and the
	 *			control bit to indicate if the value being written is data (1) or command (0).
	 */
	
	GPIOA->DIR 	 	 |=  0x3C;							// setting PA2-PA5 as outputs (interface to LCD)
	GPIOA->DATA  	 &= ~0x3C; 							// clearing the line
	GPIOA->DATA  	 |= (data & 0xF0) >> 2; // extracting the upper nibble
	
	
	/* set RS bit */
	if (control & 1)
		GPIOE->DATA	 |=  1;			// check if LCD data is data
	else
		GPIOE->DATA  &= ~1; 		// check if LCD data is command
	
	
	/* sending a high to low transition pulse on LCD enable pin (PC6) */
	GPIOC->DATA 	 |=  (1 << 6);
	delay_ms(0);
	GPIOC->DATA 	 &= ~(1 << 6);
	
	GPIOA->DIR 	 	 &= ~0x3C;	// clearing the line
}


/*	this is used to write one byte (2 chars) ----------------------------- */
void LCD_command(unsigned char command) {
	
	LCD_nibble_write(command & 0xF0, 0); // upper nibble
	LCD_nibble_write(command << 4, 0);
	
	
	// lower nibble
	if (command < 3)
		delay_ms(2);	// maximum delay of 2 ms for the first two instructions
	else
		delay_ms(1);	// maximum delay of 1 ms for all other instructions
}


void PORTS_init() {
	
	SYSCTL->	RCGCGPIO	 |= (0x01) | (0x04) | (0x10); // enables ports A C and E
	
	/* 	configure PA5 - PA2 for digital output*/
	GPIOA->AMSEL 	 &= ~0x3C;	// disables A5-A2 analog
	GPIOA->DATA 	 &= ~0x3C;	// clears A5-A2 data
	GPIOA->DIR 	 	 |=  0x3C;	// enables A5-A2 digital
	GPIOA->DEN 		 |=  0x3C;	// enables as outputs for LCD
	
	
	/* sets GPIOE pin 0 for register select (RS)*/
	GPIOE->AMSEL	 &= ~0x01;	// disables E0 analog
	GPIOE->DIR		 |=  0x01;	// enables E0 digital
	GPIOE->DEN		 |=  0x01;	// enables as outputs for LCD
	GPIOE->DATA		 |=  0x01;	// sets E0; sets RS pin as data first
	
	/*	set PC6 for LCD EN pin*/
	GPIOC->AMSEL 	 &= ~0x40;
	GPIOC->DIR		 |=  0x40;
	GPIOC->DEN 		 |=  0x40;
	GPIOC->DATA 	 &= ~0x40;
	
}

void LCD_init() {
	
	/*
	can display ASCII
	can display in one of the 16 columns or 2 rows
	pins D7 to D0 can be used, as in the full data width
	
	some commands:
	Command		Function
	0x01			Clear display screen
	0x02			Return cursor to home i.e starting position
	0x06			Shift the cursor one position right
	0x0F			Turn on LCD display
	0x80			Takes cursor to beginning of first row
	0xC0			Takes cursor to beginning of Second row
	0x28			Select 4-Bit Mode of LCD
	0x38			Select 8-Bit Mode of LCD
	
	LCD pins D4-D7 are connected to PA2-PA5 of the Tiva launchpad
	LCD pin RS 			is connected to PE0 of the Tiva launchpad
	LCD pin E 			is connected to PC6 of the Tiva launchpa
	*/
	
	PORTS_init();		// this declarees all of the pins necessary (ports A C & E)
	/*
	proper procedure for initialization goes like this. Then it can be configured.
			Wait for about 20 ms
			Send the first value of 0x30
			Wait for 5 ms
			Send the second value of 0x30
			Wait for 1 ms
			Send the third value of 0x30
			Wait for 1 ms
	 */
	
	// initialization sequence
	delay_ms(20);
	LCD_nibble_write(0x30, 0);
	delay_ms(5);
	LCD_nibble_write(0x30, 0);
	delay_ms(1);
	LCD_nibble_write(0x30, 0);
	delay_ms(1);
	
	
	// configuration
	LCD_nibble_write(0x20, 0);	// use 4-bit data mode
	delay_ms(1);
	LCD_command(0x28);					// set 4-bit data, 2-line, 5x7 font
	LCD_command(0x06);					// move cursor right
	LCD_command(0x01);					// clear screen, move cursor to home
	LCD_command(0x0F);					// turn on LCD
	
}


void LCD_data(char data) {
	/* we're sending data through the register select data */
	
	LCD_nibble_write(data & 0xF0, 1);		// write the upper nibble
	LCD_nibble_write(data << 4, 	1);		// write the lower nibble
	delay_ms(1);
}