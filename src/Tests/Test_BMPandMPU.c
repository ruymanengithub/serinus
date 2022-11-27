/*
 * 
 * Testing both I2C modules at a time, BMP180 and MPU6050.
 * 
 * R. Azzollini
 * November 2022
 * 
 */

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "../BMP180/bmp180.h"
#include "../MPU6050/mpu6050.h"
#include "../i2c/i2c.h"

int main(void){

    i2c_inst_t* I2C_ID = i2c1;

    i2c_setup(MPU_SDA_PIN, MPU_SCL_PIN);
    mpu6050_reset(I2C_ID);

    i2c_setup(BMP_SDA_PIN, BMP_SCL_PIN);

    int16_t raccel[3], rgyro[3], rtempMPU;

    float accel[3], gyro[3], gyrobuffer, tempMPU;
    float mgyro[3];
    for (int i = 0; i < 3; i++) {
            mgyro[i] = 0.0;
    }

    int16_t rtempBMP, rpressure;
    float pressure, tempBMP;


    while (1) {

        // MPU6050

        mpu6050_read_raw(I2C_ID, raccel, rgyro, &rtempMPU);
        mpu6050_read_cal(I2C_ID, accel, gyro, &tempMPU);

        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("(Raw) Acc. X = %i, Y = %i, Z = %i\n", raccel[0], raccel[1], raccel[2]);
        printf("(Raw) Gyro. X = %i, Y = %i, Z = %i\n", rgyro[0], rgyro[1], rgyro[2]);
        // Temperature is simple so use the datasheet calculation to get deg C.
        // Note this is chip temperature.
        printf("Temp. = %i\n", rtempMPU);

        //printf("Gyro. Y = %d\n", gyro[2]);

        for (int i = 0; i < 3; i++) {
            gyrobuffer = (fabs(gyro[i])>fabs(mgyro[i])) ? gyro[i] : mgyro[i];
            mgyro[i] = gyrobuffer;
        }

        printf("Acc. X = %f, Y = %f, Z = %f\n", accel[0], accel[1], accel[2]);
        printf("AbsMax(Gyro.) X = %f, Y = %f, Z = %f\n", mgyro[0], mgyro[1], mgyro[2]);
        printf("Temp. = %f\n", tempMPU);

        // BMP180

        bmp180_read_raw(I2C_ID, &rpressure, &rtempBMP);
        bmp180_read_cal(I2C_ID, &pressure, &tempBMP);

        sleep_ms(500);

    }


}