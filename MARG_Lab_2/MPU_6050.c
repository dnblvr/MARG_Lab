
#include "MPU_6050.h"

#include "I2C.h"

#define MPU_ADDR 0x68
	
void MPU6050_Init()	{
	
 I2C3_Wr(MPU_ADDR, SMPLRT_DIV,   0x07);
 I2C3_Wr(MPU_ADDR, PWR_MGMT_1,   1);
 I2C3_Wr(MPU_ADDR, CONFIG,       0x00);
 I2C3_Wr(MPU_ADDR, ACCEL_CONFIG, 0x00);
 I2C3_Wr(MPU_ADDR, GYRO_CONFIG,  0x18);
 I2C3_Wr(MPU_ADDR, INT_ENABLE,   0x00);
	
}