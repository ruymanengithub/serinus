/* 
* HC-SR04 functions.
*
* R. Azzollini
* June 2023.
*/

#include "ultrasound.h"

// Define the GPIO pins for the ultrasound module
uint8_t TRIGGER_PIN  = 0;
uint8_t ECHO_PIN = 1;

void ultrasound_setPins(uint8_t trig, uint8_t echo){
    TRIGGER_PIN = trig;
    ECHO_PIN = echo;
}

void ultrasound_init(void){
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

float ultrasound_readDistance(void){
    // Returns distance in cm
    
    uint64_t start = time_us_64();
    uint64_t end = 0;
    uint64_t timeout;

    gpio_put(TRIGGER_PIN, 1);
    sleep_us(10);
    gpio_put(TRIGGER_PIN, 0);
    
    while ((gpio_get(ECHO_PIN) == 0))
        start = time_us_64();

    timeout = start + HC_TIMEOUT;

    while ((gpio_get(ECHO_PIN) == 1) && (end < timeout))
        end = time_us_64();
    
    if (end >= timeout)
        return -1;    
    
    float distance = (end - start) / 2 * SOUND_SPEED / 1.E6; // cm
    //float distance = (end - start) / 58.0; // cm
    //float distance = (end - start);
    if ((distance > HC_MAX_DISTANCE) || (distance < HC_MIN_DISTANCE))
        return -1;
    
    return distance;
}

float ultrasound_readDistanceAvg(uint8_t n){
    // Returns average distance in cm
    
    float distance = 0;
    for (uint8_t i = 0; i < n; i++){
        distance += ultrasound_readDistance();
        sleep_ms(70);
    }
    distance /= n;
    return distance;
}

