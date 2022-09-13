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

    long state;
    //Stream* stream; // pointer
    long last;
    int MaxAttempts;
    int buffer[hPROTOCOL_LENGTH];
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
uint16_t FSIBus_readChannel(uint8_t channelNr, FSKY *fsky);


/*
class FlySkyIBus
{
  public:
    //static void _on_uart_rx(void);
    uint16_t readChannel(uint8_t channelNr);
    uint32_t bytes_rxed = 0;

    FlySkyIBus(uint8_t UART_RX_PIN) { 
      // Constructor

      // Set up our UART with a basic baud rate.
      uart_init(this->UART_ID, this->BAUD_RATE);
      // Set the TX and RX pins by using the function select on the GPIO
      // Set datasheet for more information on function select
      //gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
      gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
      // Set UART flow control CTS/RTS, we don't want these, so turn them off
      uart_set_hw_flow(this->UART_ID, false, false);
      // Set our data format
      uart_set_format(this->UART_ID, this->DATA_BITS, 
          this->STOP_BITS, PARITY);
      // Turn ON? FIFO's - we want to read the whole buffer?
      uart_set_fifo_enabled(this->UART_ID, true);

    }

    void set_irq(void) { 
      // Set up a RX interrupt
      // We need to set up the handler first
      // Select correct interrupt for the UART we are using
      int UART_IRQ = this->UART_ID == uart1 ? UART1_IRQ : UART0_IRQ;
      // And set up and enable the interrupt handlers

      irq_handler_t on_uart_rx = this::_on_uart_rx;

      irq_set_exclusive_handler(UART_IRQ, *on_uart_rx);
      irq_set_enabled(UART_IRQ, true);
     
      // Now enable the UART to send interrupts - RX only
      uart_set_irq_enables(this->UART_ID, true, false);

    }

    // RX interrupt handler
    void _on_uart_rx(void) {
    //void on_uart_rx(void) {
      while (uart_is_readable(this->UART_ID)) {
        //uint8_t ch = uart_getc(UART_ID);
        uart_read_blocking(this->UART_ID, this->buffer, PROTOCOL_LENGTH);
        this->bytes_rxed +=PROTOCOL_LENGTH;
      };
    };


  private:

    uart_inst_t* UART_ID = uart1;
    static const uint BAUD_RATE = 115200;
    static const uint8_t DATA_BITS = 8;
    static const uint8_t STOP_BITS = 1;
    static const uint8_t PROTOCOL_LENGTH = 0x20;
    static const uint8_t PROTOCOL_OVERHEAD = 3; // <len><cmd><data....><chkl><chkh>
    static const uint8_t PROTOCOL_TIMEGAP = 3; // Packets are received very ~7ms so use ~half that for the gap
    static const uint8_t PROTOCOL_CHANNELS = 6;
    static const uint8_t PROTOCOL_COMMAND40 = 0x40; // Command is always 0x40 ??

    uint8_t state;
    //Stream* stream; // pointer
    uint32_t last;
    uint8_t buffer[PROTOCOL_LENGTH];
    uint8_t ptr;
    uint8_t len;
    uint16_t channel[PROTOCOL_CHANNELS];
    uint16_t chksum;
    uint8_t lchksum;

    enum State
    {
      GET_LENGTH,
      GET_DATA,
      GET_CHKSUML,
      GET_CHKSUMH,
      DISCARD,
    };

};
*/

/*
// RX interrupt handler
void on_uart_rx(void) {
  FlySkyIBus::_on_uart_rx();
};
*/

//extern FlySkyIBus IBus; 
