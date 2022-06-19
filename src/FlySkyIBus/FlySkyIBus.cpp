/*
 * Simple interface to the Fly Sky IBus RC system.
 * inspired on: https://gitlab.com/timwilkinson/FlySkyIBus
 * adapted to pico sdk (by a complete cpp newbie...).
 * 
 * Author: R. Azzollini (2022)
 */

#include "FlySkyIBus.h"

FlySkyIBus IBus;


// RX interrupt handler
void FlySkyIBus::_on_uart_rx(void) {
//void on_uart_rx(void) {
    while (uart_is_readable(this->UART_ID)) {
        //uint8_t ch = uart_getc(UART_ID);
        uart_read_blocking(this->UART_ID, this->buffer, this->PROTOCOL_LENGTH);
        this->bytes_rxed +=PROTOCOL_LENGTH;
    };
};


/*
// RX interrupt handler
void on_uart_rx(void) {

};
*/