/*
* I/F to the BMP180 Pressure and Temperature module.
*
* R. Azzollini
* March 2023.
* credits to: https://github.com/tvlad1234/pico-bmp085Driv
*/

#include "bmp180.h"


int BMP_SDA_PIN = PICO_DEFAULT_I2C_SDA_PIN;
int BMP_SCL_PIN = PICO_DEFAULT_I2C_SCL_PIN;
uint8_t addrBMP = 0x77 ;
i2c_inst_t *I2C_ID_BMP = i2c_default;
uint8_t oversampling;

uint8_t __BMP180_CONTROL           = 0xF4; // 244
uint8_t __BMP180_TEMPDATA          = 0xF6; // 246
uint8_t __BMP180_PRESSUREDATA      = 0xF6; // 246
uint8_t __BMP180_READTEMPCMD       = 0x2E; // 46
uint8_t __BMP180_READPRESSURECMD   = 0x34; // 52


//#define NUM_CALIB_BYTES 22
struct BMP180_CALregs {
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

struct BMP180_CALregs calregs = {
.AC1 = 0xAA, .AC2 = 0xAC,  
.AC3 = 0xAE, .AC4 = 0xB0,  
.AC5 = 0xB2,  .AC6 = 0xB4,  
.B1 = 0xB6, .B2 = 0xB8,  
.MB = 0xBA,  .MC = 0xBC,  
.MD = 0xBE  
};

BMP180_CAL cal180 = {
.AC1 = 408, 
.AC2 = -72,  
.AC3 = -14383, 
.AC4 = 32741,  
.AC5 = 32757,  
.AC6 = 23153,  
.B1 = 6190, 
.B2 = 4,  
.MB = -32768,  
.MC = -8711,  
.MD = 2868  
};

uint8_t BMPreadI2C_1Byte(uint8_t reg)
{
  uint8_t data;
  i2c_write_blocking(I2C_ID_BMP, addrBMP, &reg, 1, true);
  i2c_read_blocking(I2C_ID_BMP, addrBMP, &data, 1, false);
  return data;
}

void BMPwriteI2C_1Byte(uint8_t a, uint8_t d)
{
  uint8_t tBuf[2];
  tBuf[0] = a;
  tBuf[1] = d;
  i2c_write_blocking(I2C_ID_BMP, addrBMP, tBuf, 2, false);
}

uint16_t BMPreadI2C_2UBytes(uint8_t reg){
    uint8_t buffer[2];
    int16_t data;
    // reads an unsiged integer
    i2c_write_blocking(I2C_ID_BMP, addrBMP, &reg, 1, true);
    i2c_read_blocking(I2C_ID_BMP, addrBMP, buffer, 2, false);
    data = buffer[1] | (buffer[0]<<8);
    //printf("\n2UBytes=%i %i\n", buffer[0], buffer[1]);
    return data;
}

int16_t BMPreadI2C_2Bytes(uint8_t reg){
    // reads an integer, 2 bytes-long
    uint8_t buffer[2];
    int16_t data;
    i2c_write_blocking(I2C_ID_BMP, addrBMP, &reg, 1, true);
    i2c_read_blocking(I2C_ID_BMP, addrBMP, buffer, 2, false);
    data = buffer[1] | (buffer[0]<<8);
    //printf("\n2UBytes=%i %i\n",buffer[0],buffer[1]);
    return data;
}


BMP180_CAL read_BMP180cal(void)
{
    BMP180_CAL cal;
    cal.AC1 = BMPreadI2C_2Bytes(calregs.AC1);
    cal.AC2 = BMPreadI2C_2Bytes(calregs.AC2);
    cal.AC3 = BMPreadI2C_2Bytes(calregs.AC3);
    
    cal.AC4 = BMPreadI2C_2UBytes(calregs.AC4);
    cal.AC5 = BMPreadI2C_2UBytes(calregs.AC5);
    cal.AC6 = BMPreadI2C_2UBytes(calregs.AC6);

    cal.B1 = BMPreadI2C_2Bytes(calregs.B1);
    cal.B2 = BMPreadI2C_2Bytes(calregs.B2);
    cal.MB = BMPreadI2C_2Bytes(calregs.MB);
    cal.MC = BMPreadI2C_2Bytes(calregs.MC);
    cal.MD = BMPreadI2C_2Bytes(calregs.MD);

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

// pin select function
void bmp180_setI2C(i2c_inst_t *i, uint16_t sda, uint16_t scl)
{
  I2C_ID_BMP = i;
  BMP_SDA_PIN = sda;
  BMP_SCL_PIN = scl;
}

uint8_t bmp180_init(uint8_t mode)
{

    i2c_init(I2C_ID_BMP, 800 * 1000);
    gpio_set_function(BMP_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BMP_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BMP_SDA_PIN);
    gpio_pull_up(BMP_SCL_PIN);

    if (mode > __BMP180_ULTRAHIGHRES)
        mode = __BMP180_ULTRAHIGHRES;
    oversampling = mode;

    if (bmp180_testcomm() != 0x55)
        return 1;
    
    return 0;
}

uint8_t bmp180_testcomm(void)
{
    uint8_t reg = 0xD0;
    uint8_t ID = 0;
    i2c_write_blocking(I2C_ID_BMP, addrBMP, &reg, 1, true); // true to keep master control of bus
    i2c_read_blocking(I2C_ID_BMP, addrBMP, &ID, 1, false);
    return ID;
}


int32_t computeB5(int32_t UT, BMP180_CAL cal180)
{
  int32_t X1 = (UT - (int32_t)cal180.AC6) * ((int32_t)cal180.AC5) >> 15;
  int32_t X2 = ((int32_t)cal180.MC << 11) / (X1 + (int32_t)cal180.MD);
  return X1 + X2;
  
}

uint16_t bmp180_readRawTemp(void)
// Reads the raw (uncompensated) temperature from the sensor.
{
    BMPwriteI2C_1Byte(__BMP180_CONTROL, __BMP180_READTEMPCMD);
    sleep_ms(5);
    uint16_t raw = BMPreadI2C_2UBytes(__BMP180_TEMPDATA);
    //printf("\nUT=%i\n", raw);
    return raw;
}

int32_t bmp180_readRawPressure(void)
// Reads the raw (uncompensated) pressure level from the sensor
{
    int32_t pressure;
    int wait;

    switch (oversampling){
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

    uint8_t pbytes[3] = {0};
    BMPwriteI2C_1Byte(__BMP180_CONTROL, __BMP180_READPRESSURECMD+(oversampling<<6));
    sleep_ms(wait); // wait N ms, N depends on resolution mode
    
    for (int i =0; i<3; i++) pbytes[i] = BMPreadI2C_1Byte(__BMP180_PRESSUREDATA+i);    

    //printf("Reading Raw Pressure MSB LSB XLSB: %i %i %i\n", 
    //    pbytes[0],pbytes[1], pbytes[2]);

    pressure = (int32_t)((pbytes[0] << 16) | (pbytes[1] << 8) | 
        pbytes[2]) >> (8 - oversampling);
    //pressure = 23843; // TEST

    //printf("\nUP=%i\n", pressure);

    return pressure;
}

float bmp180_readCompTemp(BMP180_CAL cal180, int DebugMode)
// reads and computes the compensated temperature in degrees celsius,
{
    
    //printf("\n\nReading Comp. Temp\n"); 
    int32_t UT, B5; 
    float temperature;

    // Read raw temp before aligning it with the calibration values
    if (!DebugMode) {
        UT = bmp180_readRawTemp();
        //printf("\n\nRaw temp: %i\n", UT); 
    } else {
        UT=27898;
    };


    B5 = computeB5(UT, cal180);
    temperature = (B5 + 8) >> 4;
    temperature /= 10;

    return temperature;

}

long bmp180_readCompPressure(BMP180_CAL cal180, int DebugMode)
// reads and computes the compensated pressure in pascals
{
    //printf("\n\nReading Comp. Pressure\n"); 
    int32_t UT, UP, X1, X2, X3, B3, B5, B6;
    uint32_t B4, B7;
    long pressure;

    if (!DebugMode) {
        UT = bmp180_readRawTemp(); 
        UP = bmp180_readRawPressure(); 
        //printf("\nRaw temp: %i\n", UT); 
        //printf("Raw Pressure: %i\n", UP); 
    } else {
        UT=27898;
        UP=23843;
    };


    B5 = computeB5(UT, cal180);

    B6 = B5-4000;
    X1 = ((int32_t)cal180.B2 * ((B6*B6)>>12))>>11;
    X2 = ((int32_t)cal180.AC2*B6)>>11;
    X3 = X1+X2;
    B3 = ((((int32_t)cal180.AC1*4+X3)<<oversampling)+2)/4;
    
    X1 = ((int32_t)cal180.AC3*B6)>>13;
    X2 = ((int32_t)cal180.B1*((B6*B6)>>12))>>16;
    X3 = ((X1+X2)+2)>>2;

    B4 = ((uint32_t)cal180.AC4 * (uint32_t) (X3+32768))>>15;
    B7 = ((uint32_t) UP-B3)*(uint32_t)(50000UL >>oversampling);
    
    if (B7<0x80000000)
    {
        pressure=(B7*2)/B4;
    } else {
        pressure= (B7/B4)*2;
    } 
    
    X1 = (pressure>>8)*(pressure>>8);
    X1 = (X1*3038)>>16;
    X2 = (-7357*pressure)>>16;
    pressure = pressure+((X1+X2+(int32_t)3791)>>4);
    

    return pressure;

}

double getAltitude(float Pressure)
// implements the international barometric formula
{
    double p0 = 101325.0; // Pa
    double power = 1./5.255;
    double altitude = 44330.0 * (1.-pow((Pressure/p0), power));

    return altitude;
}


