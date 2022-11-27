/*
* I/F to the BMP180 Pressure and Temperature module.
*
* R. Azzollini
* November 2022.
*/

#include "bmp180.h"

int BMP_SDA_PIN = 2;
int BMP_SCL_PIN = 3;

// By default these devices  are on bus address 0x20
int BMPaddr = 0x20;  // 32

void bmp180_read_raw(i2c_inst_t* IC2_ID, int16_t* pressure, int16_t* temp)
{
    *pressure = 0;
    *temp = 0;
}


void bmp180_read_cal(i2c_inst_t* IC2_ID, float* pressure, float* temp)
{
    *pressure = 0.0;
    *temp = 0.0;
}

