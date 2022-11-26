/*
*
* Library for RPI-PICO to interact with MPU6050.
*
* R. Azzollini
*
* Date: 07.2022
*/

#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3
#define FS_SEL 0
#define AFS_SEL 2

#define ACCEL_CONFIG_REG 0x1C // register
#define GYRO_CONFIG_REG 0x1B // register

#define SELF_TEST_X 0x0D
#define SELF_TEST_Y 0X0E
#define SELF_TEST_Z 0X0F
#define SELF_TEST_A 0X10

// By default these devices  are on bus address 0x68
static int addr = 0x68;  // 104


void i2c_setup(i2c_inst_t* I2C_ID);

uint8_t readI2CByte(i2c_inst_t* I2C_ID, uint8_t subAddress);

void mpu6050_selftest(i2c_inst_t* I2C_ID, float destination[6]);

void mpu6050_reset(i2c_inst_t* I2C_ID);

void mpu6050_read_raw(i2c_inst_t* I2C_ID, int16_t accel[3], int16_t gyro[3], int16_t *temp);

void mpu6050_read_cal(i2c_inst_t* I2C_ID, float accel[3], float gyro[3], float *temp);

