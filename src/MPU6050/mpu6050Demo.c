/**
* 
* author: R. Azzollini 
* 2022
*/

#include <stdio.h>
#include "mpu6050.h"


int main() {

    i2c_inst_t* I2C_ID = i2c1;
    

    stdio_init_all();
    printf("\nHello, MPU6050! Reading raw data from Registers...\n");
    
    i2c_setup(I2C_ID);
    mpu6050_reset(I2C_ID);


    float SelfTest[6];

    mpu6050_selftest(I2C_ID, SelfTest);

    printf("x-axis self test: acceleration trim within : %f pc of factory value\n", SelfTest[0]);
    printf("y-axis self test: acceleration trim within : %f pc of factory value\n", SelfTest[1]);
    printf("z-axis self test: acceleration trim within : %f pc of factory value\n", SelfTest[2]);
    printf("x-axis self test: gyration trim within : %f pc of factory value\n", SelfTest[3]);
    printf("y-axis self test: gyration trim within : %f pc of factory value\n", SelfTest[4]);
    printf("z-axis self test: gyration trim within : %f pc of factory value\n", SelfTest[5]);

    /*
    int16_t racc1[3], rgyro1[3], rtemp;
    mpu6050_read_raw(I2C_ID, racc1, rgyro1, &rtemp);
    mpu6050_selftest(I2C_ID);
    int16_t racc2[3], rgyro2[3];
    mpu6050_read_raw(I2C_ID, racc2, rgyro2, &rtemp);

    printf("DAcc. X = %d, Y = %d, Z = %d\n", racc2[0], 
                                    racc2[1], racc2[2]);
    printf("DGyro. X = %d, Y = %d, Z = %d\n", rgyro2[0], 
                                    rgyro2[1], rgyro2[2]);


    printf("DAcc. X = %d, Y = %d, Z = %d\n", racc2[0]-racc1[0], 
                                    racc2[1]-racc1[1], racc2[2]-racc2[1]);
    printf("DGyro. X = %d, Y = %d, Z = %d\n", rgyro2[0]-rgyro1[0], 
                                    rgyro2[1]-rgyro1[1], rgyro2[2]-rgyro1[2]);
    */

    float accel[3], gyro[3], temp;

    /*    

    while (1) {
        
        mpu6050_read_cal(I2C_ID, acceleration, gyro, &temp);

        

        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
        // Temperature is simple so use the datasheet calculation to get deg C.
        // Note this is chip temperature.
        printf("Temp. = %f\n", (temp / 340.0) + 36.53);


        //printf("Gyro. Y = %d\n", gyro[2]);


        printf("Acc. X = %f, Y = %f, Z = %f\n", accel[0], accel[1], accel[2]);
        printf("Gyro. X = %f, Y = %f, Z = %f\n", gyro[0], gyro[1], gyro[2]);
        printf("Temp. = %f\n", temp);


        sleep_ms(500);

    }
    */


//#endif
    return 0;
}

