/*
*
* Library for RPI-PICO to interact with HC-SR04 ultrasound module.
*
* R. Azzollini
*
* Date: 06.2023
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Define the time-out for the ultrasound module
#define HC_TIMEOUT  35000 // us, 1.5 x (2 x) max distance / speed of sound

// Define the speed of sound in air
#define SOUND_SPEED  34300 // cm/s

// Define the maximum distance to measure
#define HC_MAX_DISTANCE 400 // cm

// Define the minimum distance to measure
#define HC_MIN_DISTANCE 2 // cm


void ultrasound_setPins(uint8_t trig, uint8_t echo);

void ultrasound_init(void);

float ultrasound_readDistance(void);

float ultrasound_readDistanceAvg(uint8_t n);

