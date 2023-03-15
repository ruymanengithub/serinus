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
#include <stdlib.h>
#include <time.h>

/*

int old_main() {

    stdio_init_all();

    srand(time(NULL));
    int r = rand();

    i2c_inst_t* I2C_ID = i2c1;

    i2c_init(I2C_ID, 400 * 1000);
    
    // `time_t` is an arithmetic time type
    // time_t now;

    printf("\nHello, BMP180!\n");
    // Convert to local time format and print to stdout
    // printf("Random = %i\n", r);
    
    i2c_setup(BMP_SDA_PIN, BMP_SCL_PIN);

    int32_t rawtemp;
    int32_t rawpressure;
    float temperature;
    long pressure;
    int BMP180mode = 2; // standard=1?
    bool DebugMode = 0;
    uint8_t ID = 0;

    BMP180_CAL cal180 = read_BMP180cal(I2C_ID);
    printBMP180cal(cal180);
    bmp180_testcomm(I2C_ID, &ID);
    printf("ID = %i\n",ID);

    //rawtemp = bmp180_readRawTemp(I2C_ID);
    //rawpressure = bmp180_readRawPressure(I2C_ID, BMP180mode);

    //printf("\n\nRaw temp: %i     ", rawtemp);
    //printf("Raw Pressure: %i\n", rawpressure);
    
    temperature = bmp180_readCompTemp(I2C_ID, cal180, DebugMode);
    printf("Temperature: %.2f C, ", temperature);
    
    pressure = bmp180_readCompPressure(I2C_ID, BMP180mode, cal180, DebugMode);
    printf("Pressure: %i Pa\n", pressure);


    for(int i=0;i<3;i++){


        temperature = bmp180_readCompTemp(I2C_ID, cal180, DebugMode);
        printf("Temperature: %.2f C, ", temperature);

        pressure = bmp180_readCompPressure(I2C_ID, BMP180mode, 
            cal180, DebugMode);
        printf("Pressure: %i Pa\n", pressure);
        
        sleep_ms(2000);

    }
    
    
    return 0;
}

*/

int main(){


    int DebugMode = 0;

    stdio_init_all();

    uint8_t mode = __BMP180_STANDARD;

    i2c_inst_t* I2C_ID = i2c1;

    bmp180_setI2C(I2C_ID, 2, 3);
    bmp180_init(mode);

    BMP180_CAL cal180 = read_BMP180cal();

    printf("\nHello, BMP180!\n");
    
    float temperature;
    int32_t pressure;
    
    while(1){


        temperature = bmp180_readCompTemp(cal180, DebugMode);
        printf("Temperature: %.2f C\n", temperature);

        pressure = bmp180_readCompPressure(cal180, DebugMode);
        printf("Pressure: %i Pa\n", pressure);
        
        sleep_ms(2000);

    }
    

    
    return 0;
}
