
#include "QMC5883L.h"
#include "I2C.h"

void QMC5883L_init() {		
	
	I2C3_Wr(QMC_ADDR, SET_RESET_PERIOD_REG, 0x01);	// recommended by the datasheet		
	
	//I2C3_Wr(ADDR, CTRL_REG_1, 0x0D); // 0b11000101
	I2C3_Wr(QMC_ADDR, CTRL_REG_1, 0x01); // 0b11000101
	
	//I2C3_Wr(ADDR, CTRL_REG_2, 0xC1);
	I2C3_Wr(QMC_ADDR, CTRL_REG_2, 0x01);
	
}