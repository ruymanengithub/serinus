/*
*
* Library for RPI-PICO to interact with BMP180 Pressure/Temperature sensor.
*
* R. Azzollini
*
* Date: 03.2023
*/

#include <math.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


extern uint8_t addrBMP;


// Operting Modes
enum BMP180_modes{
    __BMP180_ULTRALOWPOWER, // 0
    __BMP180_STANDARD, // 1
    __BMP180_HIGHRES, // 2
    __BMP180_ULTRAHIGHRES // 3
};


typedef struct BMP180_CAL {
int32_t AC1, AC2, AC3; 
uint32_t AC4, AC5, AC6; // = 0xB0;  // R   Calibration data (16 bits)
int32_t B1, B2, MB, MC, MD;  // = 0xB6;  // R   Calibration data (16 bits)
}BMP180_CAL;

BMP180_CAL read_BMP180cal(void);

void bmp180_setI2C(i2c_inst_t* I2C_ID, uint16_t sda, uint16_t scl);
uint8_t bmp180_init(uint8_t mode);

void printBMP180cal(BMP180_CAL cal);
uint8_t bmp180_testcomm(void);

int32_t bmp180_readRawTemp(void);
float bmp180_readCompTemp(BMP180_CAL cal180, int DebugMode);

int32_t bmp180_readRawPressure(void);
long bmp180_readCompPressure(BMP180_CAL cal180, int DebugMode);

double getAltitude(float Pressure);

