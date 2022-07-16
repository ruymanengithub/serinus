/*
*
* Demo of PWM control with RPI-PICO
*
* R. Azzollini
* 07.2022
* 
*/

#include "pwm.h"
#include "hardware/irq.h"

#define LED_PIN 15

uint PWMPINS[] = {
    15,
    17,
    19,
    21
};

void on_pwm_wrap(void) {
    static int fade = 0;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(LED_PIN));

    if (going_up) {
        ++fade;
        if (fade > 255) {
            fade = 255;
            going_up = false;
        }
    } else {
        --fade;
        if (fade < 0) {
            fade = 0;
            going_up = true;
        }
    }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
    pwm_set_gpio_level(LED_PIN, fade * fade);
    printf("GPIO_LEVEL = %i\n", fade*fade);
    
}

int main() {

    stdio_init_all();
    printf("\nPWM example through MOSFET\n");

    setup_pwm(LED_PIN);


    // configuration of LED pin
    // Figure out which slice we just connected to the LED pin
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

   while (1)
    {
        tight_loop_contents();
    }

}