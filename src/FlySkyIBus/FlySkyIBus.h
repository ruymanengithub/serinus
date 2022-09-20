/*
 * Simple interface to the Fly Sky IBus RC system.
 * inspired on: https://gitlab.com/timwilkinson/FlySkyIBus (Arduino)
 * adapted to Raspberry Pi PICO SDK (by a complete C/C++ noob...).
 * 
 * Author: R. Azzollini (2022)
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/timer.h"


/*
#define PARITY UART_PARITY_NONE
#define UART_ID uart1
#define BAUD_RATE 115200
*/

#define hPROTOCOL_LENGTH 0x20
#define hPROTOCOL_OVERHEAD 
#define hPROTOCOL_CHANNELS 6
#define PROTOCOL_COMMAND40 0x40

/*
//void on_uart_rx;
void on_uart_rx(void) {
    while (uart_is_readable(this->UART_ID)) {
        //uint8_t ch = uart_getc(UART_ID);
        uart_read_blocking(this->UART_ID, this->buffer, this->PROTOCOL_LENGTH);
        this->bytes_rxed +=PROTOCOL_LENGTH;
    };
};
*/

enum State
{
  GET_LENGTH,
  GET_DATA,
  GET_CHKSUML,
  GET_CHKSUMH,
  DISCARD,
};

 

typedef struct FSKY {

    uart_inst_t* UART_ID; // = uart1;
    unsigned long BAUD_RATE; // = 115200;
    int DATA_BITS; // = 8;
    int STOP_BITS; //  = 1;
    int PARITY;
    int PROTOCOL_LENGTH; // = PROTOCOL_LENGTH;
    int PROTOCOL_OVERHEAD; //  = 3; // <len><cmd><data....><chkl><chkh>
    int PROTOCOL_TIMEGAP; //  = 3; // Packets are received every ~7ms so use ~half that for the gap
    int PROTOCOL_CHANNELS; // = 6;
    //uint8_t PROTOCOL_COMMAND40; // = 0x40; // Command is always 0x40 ??

    int state;
    //Stream* stream; // pointer
    long last;
    int MaxAttempts;
    int buffer[29];
    int ptr;
    int len;
    long channel[hPROTOCOL_CHANNELS];
    long chksum;
    long lchksum;
    long bytes_rxed;
    int attempts;
    int uartReadable;

} FSKY;

//void on_uart_rx(void);
void FSIBus_Init(int UART_RX_PIN, FSKY *fsky);
void FSIBus_Read(FSKY *fsky);
void FSIBus_Read_Full(FSKY *fsky);
uint16_t FSIBus_readChannel(uint8_t channelNr, FSKY *fsky);


