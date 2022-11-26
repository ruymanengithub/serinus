
/* 

   Example code to read data from the atm. pressure and temp. sensor BMP180.
   The unit I'm using is in a module by AZDelivery.

   

   Connections on Raspberry Pi Pico board, other boards may vary.

   GP2 -> SDA on BMP180 board
   GP3 -> SCL on BMP180 board
   3.3v (pin 36) -> VCC on BMP180 board
   GND (pin 38)  -> GND on BMP180 board
*/

#include <stdio.h>
#include "bmp180.h"

int main() {

    i2c_inst_t* I2C_ID = i2c1;
    
    stdio_init_all();
    printf("\nHello, MPU6050! Reading raw data from Registers...\n");
    
    i2c_setup(I2C_ID);
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
        printf("Temp. = %i\n", rtemp);

        //printf("Gyro. Y = %d\n", gyro[2]);

        for (int i = 0; i < 3; i++) {
            gyrobuffer = (fabs(gyro[i])>fabs(mgyro[i])) ? gyro[i] : mgyro[i];
            mgyro[i] = gyrobuffer;
        }

        printf("Acc. X = %f, Y = %f, Z = %f\n", accel[0], accel[1], accel[2]);
        printf("AbsMax(Gyro.) X = %f, Y = %f, Z = %f\n", mgyro[0], mgyro[1], mgyro[2]);
        printf("Temp. = %f\n", temp);


        sleep_ms(500);

    }
    


//#endif
    return 0;
}