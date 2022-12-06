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
extern addrBMPw, addrBMPr;


// Operating Modes
//uint8_t __BMP085_ULTRALOWPOWER     = 0;
//uint8_t __BMP085_STANDARD          = 1;
//uint8_t __BMP085_HIGHRES           = 2;
//uint8_t __BMP085_ULTRAHIGHRES      = 3;

enum BMP180_modes{
    __BMP180_ULTRALOWPOWER,
    __BMP180_STANDARD,
    __BMP180_HIGHRES,
    __BMP180_ULTRAHIGHRES
};


typedef struct BMP180_CAL {
int AC1, AC2, AC3; 
uint AC4, AC5, AC6; // = 0xB0;  // R   Calibration data (16 bits)
int B1, B2, MB, MC, MD;  // = 0xB6;  // R   Calibration data (16 bits)
} BMP180_CAL;

BMP180_CAL read_BMP180cal(i2c_inst_t* I2C_ID);
void bmp180_readRawTemp(i2c_inst_t* I2C_ID, int16_t* temp);
void bmp180_readRawPressure(i2c_inst_t* I2C_ID, int16_t* pressure, 
    int BMP180mode);
int bmp180_readCompTempPressure(i2c_inst_t* I2C_ID, float* temperature, long* pressure, 
    int BMP180mode, int DebugMode);
//long bmp180_readCompPressure(i2c_inst_t* I2C_ID, int BMP180mode);

double getAltitude(float Pressure);
