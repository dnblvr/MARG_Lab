
#include "UART.h"

void uart5_init(void) {
	
	SYSCTL->RCGCUART |= 0x20;  /* enable clock to UART5 */
	SYSCTL->RCGCGPIO |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
	Delay(1);
	
	/* UART0 initialization */
	UART5->CTL = 0;         /* UART5 module disbable */
	UART5->IBRD = 104;      /* for 9600 baud rate, integer = 104 */
	UART5->FBRD = 11;       /* for 9600 baud rate, fractional = 11*/
	UART5->CC = 0;          /*select system clock*/
	UART5->LCRH = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
	UART5->CTL = 0x301;     /* Enable UART5 module, Rx and Tx */

	/* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
	GPIOE->DEN = 0x30;      /* set PE4 and PE5 as digital */
	GPIOE->AFSEL = 0x30;    /* Use PE4,PE5 alternate function */
	GPIOE->AMSEL = 0;    /* Turn off analg function*/
	GPIOE->PCTL = 0x00110000;     /* configure PE4 and PE5 for UART */
	
}

void UART5_Transmitter(unsigned char data) {
    while ((UART5->FR & (1<<5)) != 0);	/* wait until Tx buffer not full */
    UART5->DR = data;										/* before giving it another byte */
}

void print_string(char *str)	{
  while(*str) {
		UART5_Transmitter(*(str++));
	}
}


void Delay(unsigned long counter) {
	unsigned long i = 0;
	
	for(i=0; i< counter*10000; i++);
}