
#ifndef _QMC5883L_H_
	#define _QMC5883L_H_


	#define XOUT_LSB		0x00
	#define XOUT_MSB		0x01
	#define YOUT_LSB		0x02
	#define YOUT_MSB		0x03
	#define ZOUT_LSB		0x04
	#define ZOUT_MSB		0x05

	#define STATUS_REG 	0x06
	/*
	status registers
		[reserved for the rest], DOR[2], OVL[1], DRDY[0]
	DRDY (data ready register): 
		0: no new data
		1: opposite of that

	OVL flag (overflow): 
		0: normal
		1: if ANY of the three datasets is overflowing

	DOR (data skipping under continuous mode):
		0: normal
		1: data is skipped for reading
	*/

	#define TOUT_LSB		0x07
	#define TOUT_MSB		0x08

	#define CTRL_REG_1	0x09
	/*
	Addr|7   |6  |5   |4  |3   |2  |1   |0   |
	09H |OSR[1:0]|RNG[1:0]|ODR[1:0]|MODE[1:0]|

	Reg.	| Definition				| 00			| 01					| 10     	| 11			|
	------+-------------------+---------+-------------+---------+---------+
	Mode	| Mode Control			| Standby	| Continuous	| Reserve	| Reserve	|
	ODR		| Output Data Rate	| 10 Hz		| 50 Hz				| 100 Hz	| 200 Hz	|
	RNG		| Full Scale				| 2G			| 8G					| Reserve	| Reserve	|
	OSR		| Over Sample Ratio	| 512			| 256					| 128			| 64			|
	*/


	#define CTRL_REG_2 0x0A
	/*
		Interrupt enabling is controlled by register INT_ENB in control register 2. Once the interrupt is enabled, it will flag
		when new data is in Data Output Registers.
				INT_ENB: “0”: enable interrupt PIN, “1”: disable interrupt PIN
		
		Pointer roll-over function is controlled by ROL_PNT register. When the point roll-over function is enabled, the I2C
		data pointer automatically rolls between 00H ~ 06H, if I2C read begins at any address among 00H~06H.
				ROL_PNT: “0”: Normal, “1”: Enable pointer roll-over function
		
		Soft Reset can be done by changing the register SOFT_RST to set. Soft reset can be invoked at any time of any
		mode. For example, if soft reset occurs at the middle of continuous mode reading, QMC5883L immediately
		switches to standby mode due to mode register is reset to “00” in default.
				SOFT_RST: “0”: Normal“1”: Soft reset, restore default value of all registers.
				
	Table 19. Control Register 2
	Addr.|7       |6      |5 |4 |3 |2 |1 |0      |
	0AH  |SOFT_RST|ROL_PNT|  |  |  |  |  |INT_ENB|
	*/


	#define SET_RESET_PERIOD_REG 0x0B

	/*
	9.2.5 SET/RESET Period Register
	SET/RESET Period is controlled by FBR [7:0], it is recommended that the register 0BH is written by 0x01.
	Table 20. SET/RESET Period Register
	Addr.|7  |6  |5  |4  |3  |2  |1  |0|
	0BH  |SET/RESET Period FBR [7:0]   |

	*/


	#define QMC_ADDR 0x0D

	void QMC5883L_init();

#endif // _QMC5883L_H_