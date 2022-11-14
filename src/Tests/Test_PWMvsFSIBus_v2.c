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
//#include "pico/time.h"
#include "pico/stdlib.h"
#include "../PWM/pwm.h"
#include "../FlySkyIBus/FlySkyIBus.h"


const int UART_RX_PIN = 5;
const int NPWM = 4;
const int PWM_PIN[] = {15, 17, 19, 21};


int32_t millis(void)
{
    return time_us_32() / 1000;
}

int main(void){
    
    int Thrust = 0, pwmlevel = 0;
    int maxThrust = pow(2,16)-1;
    int32_t now, last, flipLight;

    stdio_init_all();
    printf("Starting TEST PWM vs FSIBus - 01.10.2022\n");

    // shall we use the RPico LED to signal receptions? Just in case...
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    FSKY fsky;
    FSIBus_Init(UART_RX_PIN, &fsky);

    for (int i=0; i<NPWM; i++) 
        setup_pwm(PWM_PIN[i]);

    last = millis();
    flipLight = 0;

    while (1){

        FSIBus_Read_thenParse(&fsky); // read into fsky the FSIBus output

        Thrust = FSIBus_readNormChannel(FSthrust, &fsky); // reading out normalized Thrust

        pwmlevel = Thrust / 100. * pow(2,16) - 1;

        now = millis();

        // let's limit the thrust within level
        pwmlevel = (pwmlevel < 0) ? 0 : pwmlevel;
        pwmlevel = (pwmlevel > maxThrust) ? maxThrust : pwmlevel;


        if (now-last>1000)
        {
            printf("Thrust, pwmlevel:%d\t%d\n", Thrust, pwmlevel);
            last = now;
            gpio_put(LED_PIN, flipLight++ % 2);

        }

        // Update PWM value
        // PWM level between 0 and 2**16-1
        for (int i=0; i<NPWM; i++)
            pwm_set_gpio_level(PWM_PIN[i], pwmlevel); // PWM level between 0 and 2**16-1


    }

}
