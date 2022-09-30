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
        //tight_loop_contents();
        FSIBus_Read_thenParse(&fsky);
        printf("attempts: %i\n", fsky.attempts);
        printf("read : %i bytes\n", fsky.bytes_rxed);
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", 
            FSIBus_readChannel(0, &fsky),
            FSIBus_readChannel(1, &fsky),
            FSIBus_readChannel(2, &fsky),
            FSIBus_readChannel(3, &fsky),
            FSIBus_readChannel(4, &fsky),
            FSIBus_readChannel(5, &fsky));

        gpio_put(LED_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_PIN, 0);

        sleep_ms(1000);


    }

};