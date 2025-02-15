
#include <TM4C123GH6PM.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "I2C.h"
#include "MPU_6050.h"
#include "QMC5883L.h"
#include "LCD.h"
//#include "UART.h"

// char msg[20];

void delay_ms(int n);

int main() {
	
	char	row_1_string[11] = {' '},
				row_2_string[11] = {' '};
	int num_elems = 11, i;
	
	
	char 	mpu_data[14],
				qmc_data[ 6];
	
  int16_t accX, 	accY, 	accZ,
					GyroX, 	GyroY, 	GyroZ,
					Temp, 
					magX, 	magY, 	magZ;
	
	float AX, AY, AZ, t, GX, GY, GZ, MX, MY, MZ;
	
	I2C3_Init();			delay_ms(100);
	MPU6050_Init();		delay_ms(100);
	QMC5883L_init();	delay_ms(100);
	LCD_init();				delay_ms(100);
	
	while (1) {
		
		LCD_command(1);	// clears the display
		
		I2C3_Rd(MPU_ADDR, ACCEL_XOUT_H, 14, mpu_data);
		delay_ms(2);
		I2C3_Rd(QMC_ADDR, XOUT_LSB, 6, qmc_data);
		
		
		accX = 	(int16_t) ( (mpu_data[ 0] << 8 ) | mpu_data[ 1] );
		accY = 	(int16_t) ( (mpu_data[ 2] << 8 ) | mpu_data[ 3] );
		accZ = 	(int16_t) ( (mpu_data[ 4] << 8 ) | mpu_data[ 5] );
		//Temp = 	(int16_t) ( (mpu_data[ 6] << 8 ) | mpu_data[ 7] );
		GyroX = (int16_t) ( (mpu_data[ 8] << 8 ) | mpu_data[ 9] );
		GyroY = (int16_t) ( (mpu_data[10] << 8 ) | mpu_data[11] );
		GyroZ = (int16_t) ( (mpu_data[12] << 8 ) | mpu_data[13] );
		
		magX = 	(int16_t) ( (qmc_data[1] << 8) | qmc_data[0] );
		magY = 	(int16_t) ( (qmc_data[3] << 8) | qmc_data[2] );
		magZ = 	(int16_t) ( (qmc_data[5] << 8) | qmc_data[4] );
		
		// Convert The Readings
		AX = (float) accX/16384.f;
		AY = (float) accY/16384.f;
		AZ = (float) accZ/16384.f;
		GX = (float) GyroX/131.f;
		GY = (float) GyroX/131.f;
		GZ = (float) GyroX/131.f;
		//t = ((float) Temp/340.00) + 36.53;		
		
		MX = (float) magZ/16384.f;
		MY = (float) magZ/16384.f;
		MZ = (float) magZ/16384.f;
		
		// -------------------------------------------------------------------
		
		sprintf(row_1_string, "AZ=%.5f", AZ);
		sprintf(row_2_string, "MZ=%.5f", MZ);
		
		for (i = 0; i < num_elems; i++) {
			LCD_data(row_1_string[i]);
		}
		LCD_command(0xC0);
		
		for (i = 0; i < num_elems; i++){
			LCD_data(row_2_string[i]);
		}
		
		delay_ms(50);
	}
}

/* delay function ------------------------------------------------------ */
void delay_ms(int n) {
	int i, j;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < 6265; j++) {}
	}
}
