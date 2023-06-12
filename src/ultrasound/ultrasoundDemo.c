/*
*
* Testing HC-SR04 ultrasound sensor with RPI-Pico.
* 
* author: R. Azzollini 
* 06.2023
*/

#include <stdio.h>
#include "ultrasound.h"

void ultrasound_demo(void){

    stdio_init_all();
    
    printf("Ultrasound demo.\n");
    printf("Setting pins...\n");
    ultrasound_setPins(27, 26);
    printf("Initializing...\n");
    ultrasound_init();
    printf("Reading distance...\n");

    while(1){
        //float distance = ultrasound_readDistanceAvg(3);
        float distance = ultrasound_readDistance();
        printf("Distance: %.2f cm\n", distance);
        sleep_ms(1000);
    }
    
}

int main(void){
    ultrasound_demo();
    return 0;
}