/*
*
* Functions to use i2c.
*
* R. Azzollini
*
* Date: 11.2022
*/

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

//const int I2C_SDA_PIN;
//const int I2C_SCL_PIN;

void i2c_setup(int I2C_SDA_PIN, int I2C_SCL_PIN);
uint8_t readI2C_1Byte(i2c_inst_t* I2C_ID, int addr, uint8_t reg);
