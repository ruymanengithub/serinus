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
#define hPROTOCOL_OVERHEAD 3
#define hPROTOCOL_CHANNELS 6
#define hPROTOCOL_COMMAND40 0x40


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
    int PROTOCOL_COMMAND40; // = 0x40; // Command is always 0x40 ??

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

} FSKY;

//void on_uart_rx(void);
void FSIBus_Init(int UART_RX_PIN, FSKY *fsky);
//void FSIBus_Read_whileParsing(FSKY *fsky);
void FSIBus_Read_thenParse(FSKY *fsky);
//void FSIBus_Read_Full(FSKY *fsky);
uint16_t FSIBus_readChannel(uint8_t channelNr, FSKY *fsky);

