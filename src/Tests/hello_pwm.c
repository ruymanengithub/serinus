/**
 * 
 * Testing the configuration of the PWM pins.
 * Also testing with the oscilloscope probing on the pins.
 * author: R. Azzollini
 */

// Output PWM signals on pins 16, 17, 18 and 19

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const uint MEASURE_PIN = 5;
//const uint PWMPIN_1 = 16;
//const uint PWMPIN_2 = 17;
//const uint PWMPIN_3 = 18;
//const uint PWMPIN_4 = 19;


const uint PWMPINS[] = {
    16,
    17,
    18,
    19
}

const float duty_cycles[] = {
    0.1f,
    0.5f,
    0.9f,
    1.f
}

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

int main() {
    stdio_init_all()
    printf("\nPWM duty cycle measurement example\n");
    const uint count_top = 1000;
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, count_top);

    // configuration of PWM pins
    for (int j = 0; j < count_of(PWMPINS); ++j) {
        uint PWMPIN = PWMPINS[j];
        pwm_init(pwm_gpio_to_slice_num(PWMPIN), &cfg, true);
        gpio_set_function(PWMPIN, GPIO_FUNC_PWM);
        float output_duty_cycle = test_duty_cycles[j];
        pwm_set_gpio_level(PWMPIN, output_duty_cycle * (count_top + 1));
    }

    /*
    // Tell GPIOs they are allocated to the PWM
    gpio_set_function(PWMPIN_1, GPIO_FUNC_PWM);
    gpio_set_function(PWMPIN_2, GPIO_FUNC_PWM);
    gpio_set_function(PWMPIN_4, GPIO_FUNC_PWM);
    gpio_set_function(PWMPIN_5, GPIO_FUNC_PWM);
    */

    while (1) {

        float measured_duty_cycle = measure_duty_cycle(MEASURE_PIN);
        printf("Measured input duty cycle = %.1f%%\n", measured_duty_cycle * 100.f);
        sleep_ms(1000)

    }
    

}
