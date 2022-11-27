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
#include "../i2c/i2c.h"

/*
static int I2C_SDA_PIN = 2;
static int I2C_SCL_PIN = 3;
static int FS_SEL = 0;
static int AFS_SEL = 2;

static int ACCEL_CONFIG_REG = 0x1C; // register
static int GYRO_CONFIG_REG = 0x1B; // register

static int SELF_TEST_X = 0x0D;
static int SELF_TEST_Y = 0X0E;
static int SELF_TEST_Z = 0X0F;
static int SELF_TEST_A = 0X10;

// By default these devices  are on bus address 0x68
static int addrMPU = 0x68;  // 104

*/

extern MPU_SDA_PIN;
extern MPU_SCL_PIN;
extern FS_SEL;
extern AFS_SEL;

extern ACCEL_CONFIG_REG; // register
extern GYRO_CONFIG_REG; // register

extern SELF_TEST;
extern SELF_TEST_Y;
extern SELF_TEST_Z;
extern SELF_TEST_A;

// By default these devices  are on bus address 0x68
extern addrMPU;  // 104


void mpu6050_selftest(i2c_inst_t* I2C_ID, float destination[6]);

void mpu6050_reset(i2c_inst_t* I2C_ID);

void mpu6050_read_raw(i2c_inst_t* I2C_ID, int16_t accel[3], int16_t gyro[3], int16_t *temp);

void mpu6050_read_cal(i2c_inst_t* I2C_ID, float accel[3], float gyro[3], float *temp);


