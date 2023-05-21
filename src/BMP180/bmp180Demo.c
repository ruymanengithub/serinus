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
#include <string.h>


int main(){


    int DebugMode = 0;

    stdio_init_all();

    //time_t mytime = time(NULL);
    //char* time_str = ctime(&mytime);
    //time_str[strlen(time_str)-1] = '\0';
    printf("Time: 19:24\n");

    uint8_t mode = __BMP180_STANDARD;

    i2c_inst_t* I2C_ID = i2c1;

    bmp180_setI2C(I2C_ID, 2, 3);
    bmp180_init(mode, 800*1000, true);

    printf("\nHello, BMP180!\n");

    BMP180_CAL cal180 = read_BMP180cal();
    printBMP180cal(cal180);
    
    uint8_t ID = bmp180_testcomm();
    printf("ID = %i\n", ID);
    
    float temperature;
    int32_t pressure;
    
    temperature = bmp180_readCompTemp(cal180, DebugMode);
    printf("Temperature: %.2f C\n", temperature);

    pressure = bmp180_readCompPressure(cal180, DebugMode);
    printf("Pressure: %i Pa\n", pressure);

    /*

    int32_t tcounter = 0;

    while(1){

        tcounter++;

        printf("%i\n", tcounter);

        temperature = bmp180_readCompTemp(cal180, DebugMode);
        printf("Temperature: %.2f C\n", temperature);

        pressure = bmp180_readCompPressure(cal180, DebugMode);
        printf("Pressure: %i Pa\n", pressure);
        
        sleep_ms(2000);

    }
    
    */
    
    return 0;
}
