/**
 * 
 * A 'dumb' program to test that thrust control via FSIbus works.
 * Using the serinus-1 08.2022 PCB as HW platform.
 * 
 * 
 * author: R. Azzollini
 */

// Output PWM signals on pin GP15

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "../PWM/pwm.h"
#include "../FlySkyIBus/FlySkyIBus.h"


#define PWM_PIN 15
#define UART_RX_PIN 5

int main(void){
    
    stdio_init_all();
    printf("Starting TEST PWM vs FSIBus - 01.10.2022");

    // shall we use the RPico LED to signal receptions? Just in case...
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    
    FSKY fsky;
    FSIBus_Init(UART_RX_PIN, &fsky);

    setup_pwm(PWM_PIN);

    int Thrust = 0, pwmlevel = 0;
    int maxThrust = pow(2,16)-1;

    while (1){

        FSIBus_Read_thenParse(&fsky); // read into fsky the FSIBus output

        Thrust = FSIBus_readNormChannel(FSthrust, &fsky); // reading out normalized Thrust

        pwmlevel = Thrust / 100. * pow(2,16) - 1;

        // let's limit the thrust within level
        pwmlevel = pwmlevel < 0 ? 0 : pwmlevel;
        pwmlevel = pwmlevel > maxThrust ? maxThrust : pwmlevel;

        // Update PWM value
        pwm_set_gpio_level(PWM_PIN, pwmlevel); // PWM level between 0 and 2**16-1


    }

}
