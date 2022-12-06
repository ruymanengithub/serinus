/*
* I/F to the BMP180 Pressure and Temperature module.
*
* R. Azzollini
* November 2022.
*/

#include "bmp180.h"

int BMP_SDA_PIN = 2;
int BMP_SCL_PIN = 3;

uint8_t __BMP180_CONTROL           = 0xF4; // 244
uint8_t __BMP180_TEMPDATA          = 0xF6; // 246
uint8_t __BMP180_PRESSUREDATA      = 0xF6; // 246
uint8_t __BMP180_READTEMPCMD       = 0x2E; // 46
uint8_t __BMP180_READPRESSURECMD   = 0x34; // 52

uint8_t addrBMPread = 0xEF;  // 239, read
uint8_t addrBMPwrite = 0xEE;  // 238, write

struct BMP180_CALregs {
    uint8_t AC1;  // R   Calibration data (16 bits)
    uint8_t AC2;  // R   Calibration data (16 bits)
    uint8_t AC3;  // R   Calibration data (16 bits)
    uint8_t AC4;  // R   Calibration data (16 bits)
    uint8_t AC5;  // R   Calibration data (16 bits)
    uint8_t AC6;  // R   Calibration data (16 bits)
    uint8_t B1;  // R   Calibration data (16 bits)
    uint8_t B2;  // R   Calibration data (16 bits)
    uint8_t MB;  // R   Calibration data (16 bits)
    uint8_t MC;  // R   Calibration data (16 bits)
    uint8_t MD;  // R   Calibration data (16 bits)
};

struct BMP180_CALregs calregs = {
.AC1 = 0xAA, .AC2 = 0xAC,  
.AC3 = 0xAE, .AC4 = 0xB0,  .AC5 = 0xB2,  .AC6 = 0xB4,  
.B1 = 0xB6, .B2 = 0xB8,  
.MB = 0xBA,  .MC = 0xBC,  .MD = 0xBE  
};


struct BMP180_CAL {
    int16_t AC1;  // R   Calibration data (16 bits)
    int16_t AC2;  // R   Calibration data (16 bits)
    int16_t AC3;  // R   Calibration data (16 bits)
    uint16_t AC4;  // R   Calibration data (16 bits)
    uint16_t AC5;  // R   Calibration data (16 bits)
    uint16_t AC6;  // R   Calibration data (16 bits)
    int16_t B1;  // R   Calibration data (16 bits)
    int16_t B2;  // R   Calibration data (16 bits)
    int16_t MB;  // R   Calibration data (16 bits)
    int16_t MC;  // R   Calibration data (16 bits)
    int16_t MD;  // R   Calibration data (16 bits)
};

struct BMP180_CAL cal180 = {
.AC1 = 408, .AC2 = -72,  
.AC3 = -14383, .AC4 = 32741,  .AC5 = 32757,  .AC6 = 23153,  
.B1 = 6190, .B2 = 4,  
.MB = -32768,  .MC = -8711,  .MD = 2868  
};

uint readI2C_2UBytes(i2c_inst_t* I2C_ID, uint8_t reg){
    // reads an unsiged integer
    i2c_write_blocking(I2C_ID, addrBMPwrite, &reg, 1, true);
    uint8_t buffer[2];
    i2c_read_blocking(I2C_ID, addrBMPread, buffer, 2, false);
    uint data = buffer[0]<<8 | buffer[1];
    return data;
}

int readI2C_2Bytes(i2c_inst_t* I2C_ID, uint8_t reg){
    // reads an integer
    i2c_write_blocking(I2C_ID, addrBMPwrite, &reg, 1, true);
    uint8_t buffer[2];
    i2c_read_blocking(I2C_ID, addrBMPread, buffer, 2, false);
    int data = buffer[0]<<8 | buffer[1];
    return data;
}


BMP180_CAL read_BMP180cal(i2c_inst_t* I2C_ID)
{
    BMP180_CAL cal;
    cal.AC1 = readI2C_2Bytes(I2C_ID, calregs.AC1);
    cal.AC2 = readI2C_2Bytes(I2C_ID, calregs.AC2);
    cal.AC3 = readI2C_2Bytes(I2C_ID, calregs.AC3);
    
    cal.AC4 = readI2C_2UBytes(I2C_ID, calregs.AC4);
    cal.AC5 = readI2C_2UBytes(I2C_ID, calregs.AC5);
    cal.AC6 = readI2C_2UBytes(I2C_ID, calregs.AC6);

    cal.B1 = readI2C_2Bytes(I2C_ID, calregs.B1);
    cal.B2 = readI2C_2Bytes(I2C_ID, calregs.B2);
    cal.MB = readI2C_2Bytes(I2C_ID, calregs.MB);
    cal.MC = readI2C_2Bytes(I2C_ID, calregs.MC);
    cal.MD = readI2C_2Bytes(I2C_ID, calregs.MD);

    return cal;
}

void printBMP180cal(BMP180_CAL cal) {
    printf("AC1 = %i\n", cal.AC1);
    printf("AC2 = %i\n", cal.AC2);
    printf("AC3 = %i\n", cal.AC3);
    printf("AC4 = %i\n", cal.AC4);
    printf("AC5 = %i\n", cal.AC5);
    printf("AC6 = %i\n", cal.AC6);
    printf("B1 = %i\n", cal.B1);
    printf("B2 = %i\n", cal.B2);
    printf("MB = %i\n", cal.MB);
    printf("MC = %i\n", cal.MC);
    printf("MD = %i\n", cal.MD);
}

void bmp180_readRawTemp(i2c_inst_t* I2C_ID, int16_t* temp)
// Reads the raw (uncompensated) temperature from the sensor.
{
    uint8_t reg;
    uint8_t regAndCmd[] = {__BMP180_CONTROL, __BMP180_READTEMPCMD};
    i2c_write_blocking(I2C_ID, addrBMPwrite, regAndCmd, 2, true); // true to keep master control of bus
    sleep_ms(5); // wait 5 ms
    reg = __BMP180_TEMPDATA;
    i2c_write_blocking(I2C_ID, addrBMPwrite, &reg, 1, true); // true to keep master control of bus
    i2c_read_blocking(I2C_ID, addrBMPread, &temp, 2, false);

}

void bmp180_readRawPressure(i2c_inst_t* I2C_ID, int16_t* pressure, int BMP180mode)
// Reads the raw (uncompensated) pressure level from the sensor
{
    uint8_t reg;
    uint8_t buffer[3];
    int wait;

    switch (BMP180mode){
        case __BMP180_ULTRALOWPOWER:
            wait=5;
            break;
        case __BMP180_HIGHRES:
            wait=14;
            break;
        case __BMP180_ULTRAHIGHRES:
            wait=26;
            break;
        case __BMP180_STANDARD:
            wait=8;
            break;
    }

    uint8_t regAndCmd[2] = {__BMP180_CONTROL, __BMP180_READPRESSURECMD+ (BMP180mode<<6)};
    i2c_write_blocking(I2C_ID, addrBMPwrite, regAndCmd, 2, true); // true to keep master control of bus
    sleep_ms(wait); // wait N ms
    reg = __BMP180_PRESSUREDATA;
    i2c_write_blocking(I2C_ID, addrBMPwrite, reg, 1, true);
    i2c_read_blocking(I2C_ID, addrBMPread, buffer, 3, false);
    pressure = ((buffer[0] << 16) + (buffer[1] << 8) + buffer[2]) >> (8 - BMP180mode);
    
}


int bmp180_readCompTempPressure(i2c_inst_t *I2C_ID, float* temperature, long* pressure, 
    int BMP180mode, int DebugMode)
// reads and computes the compensated temperature in degrees celsius,
// and the compensated pressure in pascals
{
    int16_t* UT;

    // Read raw temp before aligning it with the calibration values
    if (!DebugMode) {
        bmp180_readRawTemp(I2C_ID, UT); 
    } else {
        *UT=27898;
    };

    int16_t* UP;

    if (!DebugMode) {
        bmp180_readRawPressure(I2C_ID, UP, BMP180mode); 
    } else {
        *UP=23843;
    };

    long X1 = (*UT - cal180.AC6) * (cal180.AC5 >> 15);
    long X2 = (cal180.MC << 11) / (X1 + cal180.MD);
    long B5 = X1 + X2;
    long temp10 = ((B5 + 8) >> 4);
    
    *temperature = (float)temp10 / 10.0;

    long B6 = B5-4000;
    X1 = (cal180.B2 * (B6*(B6>>12)))>>11;
    X2 = cal180.AC2*(B6>>11);
    long X3 = X1+X2;   
    long B3 = (((cal180.AC1*4+X3)<<BMP180mode)+2)/4;
    X1 = cal180.AC3*(B6>>13);
    X2 = (cal180.B1*(B6*(B6>>12)))>>16;
    X3 = ((X1+X2)+2)/4;
    unsigned long B4 = cal180.AC4 * (unsigned long) ((X3+32768)>>15);
    unsigned long B7 = ((unsigned long) UP-B3)*(50000>>BMP180mode);
    if (B7<0x80000000)
    {
        *pressure=(B7*2)/B4;
    } else {
        *pressure= (B7/B4)*2;
    } 
    X1 = (*pressure>>8)*(*pressure>>8);
    X1 = (X1*3038)>>16;
    X2 = (-7357*(*pressure))>>16;
    *pressure = *pressure+(X1+X2+3791)>>4;

    return 0;

}


double getAltitude(float Pressure)
// implements the international barometric formula
{
    double p0 = 101325.0; // Pa
    double power = 1./5.255;
    double altitude = 44330.0 * (1.-pow((Pressure/p0), power));

    return altitude;
}


