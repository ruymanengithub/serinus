/**
 * 
 * Little library to control PWM output in RPI-PICO.
 * 
 * R. Azzollini
 * 07.2022
 * 
 */

// Output PWM signals on pins 15, 17, 19 and 21

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint MEASURE_PIN = 5;


#define COUNT_TOP 65535
#define CLKDIV 4.f

/*

const float duty_cycles[] = {
    0.1f,
    0.5f,
    0.9f,
    1.f
};

float measure_duty_cycle(uint gpio) {
    // Only the PWM B pins can be used as inputs.
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Count once for every 100 cycles the PWM B input is high
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
    pwm_config_set_clkdiv(&cfg, 100);
    pwm_init(slice_num, &cfg, false);
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    pwm_set_enabled(slice_num, true);
    sleep_ms(10);
    pwm_set_enabled(slice_num, false);
    float counting_rate = clock_get_hz(clk_sys) / 100;
    float max_possible_count = counting_rate * 0.01;
    return pwm_get_counter(slice_num) / max_possible_count;

}

*/

//void setup_pwm(uint PWMPINS[4]) {
void setup_pwm(uint PWMPIN) {

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, CLKDIV);
    pwm_config_set_wrap(&cfg, COUNT_TOP);

    // configuration of PWM pins
    //for (int j = 0; j < (int)count_of(PWMPINS); ++j) {
    //    uint PWMPIN = PWMPINS[j];
        pwm_init(pwm_gpio_to_slice_num(PWMPIN), &cfg, true);
        gpio_set_function(PWMPIN, GPIO_FUNC_PWM);
    //}
}
