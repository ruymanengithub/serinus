/*
*
* I2C aux functions.
*
* R. Azzollini
* 2022
*/


#include "i2c.h"


void i2c_setup(int I2C_SDA_PIN, int I2C_SCL_PIN) {
    // sets up i2c IF in RP Pico
    //i2c_init(I2C_ID, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Make the I2C pins available to picotool
    //bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));

}

uint8_t readI2C_1Byte(i2c_inst_t* I2C_ID, int addr, uint8_t reg){
    // reads 1 Byte
    uint8_t data;
    i2c_write_blocking(I2C_ID, addr, &reg, 1, true);
    i2c_read_blocking(I2C_ID, addr, &data, 1, false);
    return data;
}
