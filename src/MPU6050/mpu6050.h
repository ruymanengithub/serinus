/*
*
*
*
*/

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

// By default these devices  are on bus address 0x68
static int addr = 0x68;  // 104


//void i2c_setup(i2c_inst_t* I2C_ID, uint pI2C_SDA_PIN, uint pI2C_SCL_PIN) {
void i2c_setup(i2c_inst_t* I2C_ID) {

    i2c_init(I2C_ID, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));

    // configure gyro

    uint8_t gyro[] = {0x1B, 0b00011000}; // FS_SEL = 3, 2000 deg/s
    i2c_write_blocking(I2C_ID, addr, gyro, 1, true); // true to keep master control of bus

    uint8_t acc[] = {0x1C, 0b00010000}; // AFS_SEL = 2, 8 g
    i2c_write_blocking(I2C_ID, addr, acc, 1, true); // true to keep master control of bus

}


void mpu6050_reset(i2c_inst_t* I2C_ID) {
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x6B, 0x00}; // 107, 0
    i2c_write_blocking(I2C_ID, addr, buf, 2, false);
}

void mpu6050_read_raw(i2c_inst_t* I2C_ID, int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B; // 59
    i2c_write_blocking(I2C_ID, addr, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(I2C_ID, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    val = 0x43; // 67
    i2c_write_blocking(I2C_ID, addr, &val, 1, true);
    i2c_read_blocking(I2C_ID, addr, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    val = 0x41; // 65
    i2c_write_blocking(I2C_ID, addr, &val, 1, true);
    i2c_read_blocking(I2C_ID, addr, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}

