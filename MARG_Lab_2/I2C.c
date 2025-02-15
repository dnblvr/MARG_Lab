
#include "I2C.h"


void I2C3_Init() {
	
	SYSCTL->RCGCGPIO |= 0x00000008 ; // Enable the clock for port D
	SYSCTL->RCGCI2C  |= 0x00000008 ; // Enable the clock for I2C 3
	GPIOD->DEN 			 |= 0x03; // Assert DEN for port D
	
	// Configure Port D pins 0 and 1 as I2C 3
	GPIOD->AFSEL	 |= 0x00000003 ;
	GPIOD->PCTL		 |= 0x00000033 ;
	GPIOD->ODR		 |= 0x00000002 ; // SDA (PD1 ) pin as open darin
	I2C3->MCR  			= 0x0010 ; // Enable I2C 3 master function
	
	/* Configure I2C 3 clock frequency
	(1 + TIME_PERIOD ) = SYS_CLK /(2*
	( SCL_LP + SCL_HP ) * I2C_CLK_Freq )
	TIME_PERIOD = 16,000,000/(2(6+4) *100000) - 1 = 7
	*/
	I2C3->MTPR  		= 0x07;
	
}


/* Wait until I2C master is not busy and return error code */
/* If there is no error, return 0 */
static int I2C_wait_till_done(void) {
	
	while (I2C3->MCS & 1);   	/* wait until I2C master is not busy */
	return I2C3->MCS & 0xE;		/* return I2C error code */
	
}


/* Write one byte only */
/* byte write: S-(saddr+w)-ACK-maddr-ACK-data-ACK-P */
char I2C3_Wr(int peripheral_addr, char register_addr, char data) {

	char error;

	/* send slave address and starting address */
	I2C3->MSA = peripheral_addr << 1;
	I2C3->MDR = register_addr;
	I2C3->MCS = 3;                      /* S-(saddr+w)-ACK-maddr-ACK */

	error = I2C_wait_till_done();       /* wait until write is complete */
	if (error) return error;

	/* send data */
	I2C3->MDR = data;
	I2C3->MCS = 5;                      /* -data-ACK-P */
	error = I2C_wait_till_done();       /* wait until write is complete */
	while(I2C3->MCS & 0x40);            /* wait until bus is not busy */
	error = I2C3->MCS & 0xE;
	if (error) return error;

	return 0;       /* no error */

	
}


char I2C3_Rd(int peripheral_addr, char register_addr, int byteCount, char* data) {

	char error;

	if (byteCount <= 0)
		return -1;         /* no read was performed */

	/* send slave address and starting address */
	I2C3->MSA = peripheral_addr << 1;
	I2C3->MDR = register_addr;
	I2C3->MCS = 3;       /* S-(saddr+w)-ACK-maddr-ACK */
	error = I2C_wait_till_done();
	if (error)
		return error;

	/* to change bus from write to read, send restart with slave addr */
	I2C3->MSA = (peripheral_addr << 1) + 1;   /* restart: -R-(saddr+r)-ACK */

	if (byteCount == 1)             /* if last byte, don't ack */
		I2C3->MCS = 7;              /* -data-NACK-P */
	else                            /* else ack */
		I2C3->MCS = 0xB;            /* -data-ACK- */
	error = I2C_wait_till_done();
	if (error) return error;

	*data++ = I2C3->MDR;            /* store the data received */

	if (--byteCount == 0) {         /* if single byte read, done */
		while(I2C3->MCS & 0x40);    /* wait until bus is not busy */
		return 0;       /* no error */
	}

	/* read the rest of the bytes */
	while (byteCount > 1)	{
		I2C3->MCS = 9;              /* -data-ACK- */
		error = I2C_wait_till_done();
		if (error)
			return error;
		byteCount--;
		*data++ = I2C3->MDR;        /* store data received */
	}

	I2C3->MCS = 5;                  /* -data-NACK-P */
	error = I2C_wait_till_done();
	*data = I2C3->MDR;              /* store data received */
	while (I2C3->MCS & 0x40);        /* wait until bus is not busy */

	return 0;       /* no error */
}