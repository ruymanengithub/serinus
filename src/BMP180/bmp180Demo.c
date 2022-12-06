
/* 

   Example code to read data from the atm. pressure and temp. sensor BMP180.
   The unit I'm using is in a module by AZDelivery.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GP2 -> SDA 
   GP3 -> SCL 
   3.3v (pin 36) -> VCC 
   GND (pin 38)  -> GND 
*/

#include <stdio.h>
#include "bmp180.h"
#include "../i2c/i2c.h"

//BMP_SDA_PIN = 2;
//BMP_SCL_PIN = 3;

int main() {

    i2c_inst_t* I2C_ID = i2c1;

    i2c_init(I2C_ID, 400 * 1000);
    
    stdio_init_all();

    printf("\nHello, BMP180!\n");
    
    i2c_setup(BMP_SDA_PIN, BMP_SCL_PIN);

    float* temperature;
    long* pressure;
    int BMP180mode = 1; // standard
    int DebugMode = 1;

    int success = bmp180_readCompTempPressure(I2C_ID, temperature, pressure, 
        BMP180mode, DebugMode);
    
    printf("Temperature: %.2f C", *temperature);
    printf("Pressure: %i Pa", *pressure);

    /*
    while (1) {

        // ...

        sleep_ms(500);

    }
    */
    

    return 0;
}