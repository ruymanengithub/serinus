/*
 * 
 * Testing both I2C modules at a time, BMP180 and MPU6050.
 * 
 * R. Azzollini
 * May 2023
 * 
 */

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "../BMP180/bmp180.h"
#include "../MPU6050/mpu6050.h"

int main(void){

    int DebugMode = 0;
    stdio_init_all();
    int bauds = 400 * 1000;
    bool doMPU = true;

    i2c_inst_t* I2C_ID_BMP = i2c1;
    i2c_inst_t* I2C_ID_MPU = i2c1;

    // BMP180

    uint8_t BMPmode = __BMP180_STANDARD;

    bmp180_setI2C(I2C_ID_BMP, 2, 3);
    bmp180_init(BMPmode, bauds, true);

    BMP180_CAL cal180 = read_BMP180cal();

    float BMPtemp;
    int32_t BMPpressure;

    // MPU6050

    if (doMPU) {
        mpu6050_setI2C(I2C_ID_MPU, 2, 3);
        mpu6050_init(bauds, false);
    }
    int16_t raccel[3], rgyro[3], MPUrtemp;
    float accel[3], gyro[3], MPUtemp;
    

    while(1){
        
        printf("\n\n");
        BMPtemp = bmp180_readCompTemp(cal180, DebugMode);
        printf("BMP-Temperature: %.2f C\n", BMPtemp);

        BMPpressure = bmp180_readCompPressure(cal180, DebugMode);
        printf("BMP-Pressure: %i Pa\n", BMPpressure);

        if (doMPU) {

            mpu6050_read_raw(I2C_ID_MPU, raccel, rgyro, &MPUrtemp);

            mpu6050_read_cal(I2C_ID_MPU, accel, gyro, &MPUtemp);

            // These are the raw numbers from the chip, so will need tweaking to be really useful.
            // See the datasheet for more information
            printf("(Raw) Acc. X = %i, Y = %i, Z = %i\n", raccel[0], raccel[1], raccel[2]);
            printf("(Raw) Gyro. X = %i, Y = %i, Z = %i\n", rgyro[0], rgyro[1], rgyro[2]);
            // Temperature is simple so use the datasheet calculation to get deg C.
            // Note this is chip temperature.
            printf("Raw Temp. = %i\n", MPUrtemp);

            printf("Acc. X = %f, Y = %f, Z = %f\n", accel[0], accel[1], accel[2]);
            printf("Gyro. X = %f, Y = %f, Z = %f\n", gyro[0], gyro[1], gyro[2]);
            printf("Temp. = %f\n", MPUtemp);
        }

        sleep_ms(2000);

    }


}