/**
* 
* author: R. Azzollini 
*/

#include <stdio.h>
#include "mpu6050.h"


int main() {

    i2c_inst_t* I2C_ID = i2c1;
    

    stdio_init_all();
    printf("Hello, MPU6050! Reading raw data from Registers...\n");
    
    i2c_setup(I2C_ID);
    mpu6050_reset(I2C_ID);

    int16_t acceleration[3], gyro[3], temp;

    while (1) {
        
        mpu6050_read_raw(I2C_ID, acceleration, gyro, &temp);

        /*

        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
        // Temperature is simple so use the datasheet calculation to get deg C.
        // Note this is chip temperature.
        printf("Temp. = %f\n", (temp / 340.0) + 36.53);
        */

        printf("Gyro. Y = %d\n", gyro[2]);

        sleep_ms(100);
    }
//#endif
    return 0;
}

