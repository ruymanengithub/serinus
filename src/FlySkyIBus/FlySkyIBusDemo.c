/*
 * Test FlySky IBus interface on RP-PICO (RP2040).
 *  Connect FlySky-RX2A receiver to Serial1.
 */

#include "FlySkyIBus.h"

#define UART_RX_PIN 5

int main(){

    stdio_init_all();
    printf("\nHello, FS-RX2A! Reading raw data from registers via UART/ibus...\n");

    // shall we use the RPico LED to signal receptions? Just in case...
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    //FlySkyIBus IBus = FlySkyIBus(UART_RX_PIN);
    //IBus.set_irq();

    FSKY fsky;

    FSIBus_Init(UART_RX_PIN, &fsky);

    while(1) {

        FSIBus_Read(&fsky);
        printf("bytes: %i\n", fsky.bytes_rxed);
        printf("attempts: %i\n", fsky.attempts);
        printf("read a byte: %i\n", fsky.uartReadable);

        gpio_put(LED_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_PIN, 0);

        sleep_ms(1000);


    }

};