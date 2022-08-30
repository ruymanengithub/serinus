/**
 * PICODRONE
 * 
 * Written by: Ruyman Azzollini (2022)
 */


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "PID/PID.h"
#include "MPU6050/mpu6050.h"

const uint LED_PIN_OB = PICO_DEFAULT_LED_PIN; // OB = On Board
const uint LED_PIN_TR = 0;
const uint LED_PIN_TL = 1;
const uint LED_PIN_BR = 2;
const uint LED_PIN_BL = 3;
int PosLights[4] = {LED_PIN_TR, LED_PIN_TL, LED_PIN_BR, LED_PIN_BL};



void SwitchOnLights();

int main() {


    //bi_decl(bi_program_description("This is a test binary."));
    //bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    gpio_init(LED_PIN_OB);
    gpio_set_dir(LED_PIN_OB, GPIO_OUT);

    //SwitchOnLights();

    // initialize radio-RX
    // initialize IMU
    // initialize motor-controllers / PWM
    // initialize PID-controller

    while (true) {
        
        gpio_put(LED_PIN_OB, 1);

        // initialize radio-RX

        // initialize PWM

        // initialize IMU

        // initialize PIDs
        
        // read radio-RX

        //gpio_put(LED_PIN_OB, 0);

        // compute PWM frequencies x4

        //gpio_put(LED_PIN_OB, 1);

        // predict attitude

        //gpio_put(LED_PIN_OB, 0);

        // read IMU - true attitude

        //gpio_put(LED_PIN_OB, 1);

        // Feed PID

        //gpio_put(LED_PIN_OB, 0);

        // get PID correction

        //gpio_put(LED_PIN_OB, 1);

        // update PWM

        gpio_put(LED_PIN_OB, 0);
        
        //sleep_ms(25);
        
    }
}


void SwitchOnLights() {

    for (int i= 0; i<size(PosLights); i++) {

        gpio_init(PosLights[i]);
        gpio_set_dir(PosLights[i], GPIO_OUT);
        gpio_put(PosLights[i], 1);

    }
}
