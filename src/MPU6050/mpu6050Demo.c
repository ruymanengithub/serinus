/**
* 
* author: R. Azzollini 
*/

#include <stdio.h>
#include "mpu6050.h"

#define PICO_DEFAULT_I2C_SDA_PIN 2
#define PICO_DEFAULT_I2C_SCL_PIN 3
#define I2C_ID i2c1

int main() {
    stdio_init_all();
#if !defined(I2C_ID) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c/mpu6050_i2c example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else
    printf("Hello, MPU6050! Reading raw data from Registers...\n");
    //printf("%s", i2c_default);
    
    i2c_setup(I2C_ID, PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN)
    mpu6050_reset();

    int16_t acceleration[3], gyro[3], temp;

    while (1) {
        
        mpu6050_read_raw(acceleration, gyro, &temp);

        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
        // Temperature is simple so use the datasheet calculation to get deg C.
        // Note this is chip temperature.
        printf("Temp. = %f\n", (temp / 340.0) + 36.53);

        sleep_ms(500);
    }

#endif
    return 0;
}

