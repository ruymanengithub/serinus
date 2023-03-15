/*
*
* Testing mpu6050 acc/gyro sensor with RPI-Pico.
* 
* author: R. Azzollini 
* 07.2022
*/

#include <stdio.h>
#include "mpu6050.h"

void run_selftest(i2c_inst_t* I2C_ID) {

    float SelfTest[6];

    mpu6050_selftest(I2C_ID, SelfTest);

    printf("x-axis self test: acceleration trim within : %f %% of factory value\n", SelfTest[0]);
    printf("y-axis self test: acceleration trim within : %f %% of factory value\n", SelfTest[1]);
    printf("z-axis self test: acceleration trim within : %f %% of factory value\n", SelfTest[2]);
    printf("x-axis self test: gyration trim within : %f %% of factory value\n", SelfTest[3]);
    printf("y-axis self test: gyration trim within : %f %% of factory value\n", SelfTest[4]);
    printf("z-axis self test: gyration trim within : %f %% of factory value\n", SelfTest[5]);


}


int main() {

    stdio_init_all();

    i2c_inst_t* I2C_ID = i2c1;
    
    printf("\nHello, MPU6050! Reading raw data from Registers...\n");
    
    i2c_init(I2C_ID, 400 * 1000);

    //i2c_setup(MPU_SDA_PIN, MPU_SCL_PIN);
    i2c_setup(10,11);
    mpu6050_reset(I2C_ID);
    
    //run_selftest(I2C_ID);

    int16_t raccel[3], rgyro[3], rtemp;

    float accel[3], gyro[3], gyrobuffer, temp;
    float mgyro[3];
    for (int i = 0; i < 3; i++) {
            mgyro[i] = 0.0;
    }
    
    while (1) {

        mpu6050_read_raw(I2C_ID, raccel, rgyro, &rtemp);

        mpu6050_read_cal(I2C_ID, accel, gyro, &temp);


        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("(Raw) Acc. X = %i, Y = %i, Z = %i\n", raccel[0], raccel[1], raccel[2]);
        printf("(Raw) Gyro. X = %i, Y = %i, Z = %i\n", rgyro[0], rgyro[1], rgyro[2]);
        // Temperature is simple so use the datasheet calculation to get deg C.
        // Note this is chip temperature.
        printf("Raw Temp. = %i\n", rtemp);

        //printf("Gyro. Y = %d\n", gyro[2]);

        for (int i = 0; i < 3; i++) {
            gyrobuffer = (fabs(gyro[i])>fabs(mgyro[i])) ? gyro[i] : mgyro[i];
            mgyro[i] = gyrobuffer;
        }

        printf("Acc. X = %f, Y = %f, Z = %f\n", accel[0], accel[1], accel[2]);
        printf("AbsMax(Gyro.) X = %f, Y = %f, Z = %f\n", mgyro[0], mgyro[1], mgyro[2]);
        printf("Temp. = %f\n", temp);


        sleep_ms(2000);

    }

//#endif
    return 0;
}

