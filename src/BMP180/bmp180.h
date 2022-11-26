/*
*
* Library for RPI-PICO to interact with BMPU180.
*
* R. Azzollini
*
* Date: 11.2022
*/

#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3
