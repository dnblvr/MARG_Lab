
#ifndef _I2C_H_
#define _I2C_H_

#include <TM4C123GH6PM.h>


void I2C3_Init(void);
static int I2C_wait_till_done(void);
char I2C3_Wr(int peripheral_addr, char register_addr, char data);
char I2C3_Rd(int peripheral_addr, char register_addr, int byteCount, char* data);


#endif // _I2C_H_ end