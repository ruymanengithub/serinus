/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
//#include "hardware/irq.h"

#define UART_ID uart1
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define BUF_LEN 32

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
//#define UART_TX_PIN 0
#define UART_RX_PIN 5

//static int chars_rxed = 0;

/*
// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        chars_rxed++;
    }
}
*/


static void read_buffer(uint8_t *buf, uint16_t len) {
    uart_read_blocking(UART_ID, buf, len);
}



int main() {

    stdio_init_all();

    printf("Hello, FS-RX2A! Reading raw data from registers via UART/ibus...\n");

    // shall we use the RPico LED to signal receptions? Just in case...
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    //gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    //uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    //int UART_IRQ = UART_ID == uart1 ? UART1_IRQ : UART0_IRQ;

    // And set up and enable the interrupt handlers
    //irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    //irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    //uart_set_irq_enables(UART_ID, true, false);
    //uart_set_irq_enables(UART_ID, false, false); // no interrupts until we know what it means

    uint8_t buffer[BUF_LEN];

    while (1){
        //tight_loop_contents(); // don't know what is this

        read_buffer(buffer, BUF_LEN);

        if (buffer[0] == 0x20){
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            gpio_put(LED_PIN, 0);
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            gpio_put(LED_PIN, 0);
            
        }

        sleep_ms(1000);

    }

    return 0;

}


