/*
*
* Library for RPI-PICO to interact with MPU6050.
*
* R. Azzollini
*
* Date: 07.2022
*/

#include <math.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"



// By default these devices  are on bus address 0x68
extern uint8_t addrMPU;  // 104


void mpu6050_setI2C(i2c_inst_t* I2C_ID, uint8_t sda, uint8_t scl);
uint8_t mpu6050_init();

void mpu6050_selftest(i2c_inst_t* I2C_ID, float destination[6]);

void mpu6050_read_raw(i2c_inst_t* I2C_ID, int16_t accel[3], int16_t gyro[3], int16_t *temp);

void mpu6050_read_cal(i2c_inst_t* I2C_ID, float accel[3], float gyro[3], float *temp);


