/*
*
* Library for RPI-PICO to interact with BMP180.
*
* R. Azzollini
*
* Date: 11.2022
*/

#include <math.h>
#include "pico/stdlib.h"
#include "../i2c/i2c.h"

extern BMP_SDA_PIN, BMP_SCL_PIN;
extern addrBMP;

void bmp180_read_raw(i2c_inst_t* IC2_ID, int16_t* pressure, int16_t* temp);
void bmp180_read_cal(i2c_inst_t* IC2_ID, float* pressure, float* temp);

