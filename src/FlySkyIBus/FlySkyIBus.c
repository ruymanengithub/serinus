/*
 * Simple interface to the Fly Sky IBus RC system.
 * inspired on: https://gitlab.com/timwilkinson/FlySkyIBus
 * Translated to C and adapted to PICO sdk (by a complete C/C++ newbie...).
 * 
 * Author: R. Azzollini (2022)
 */

#include "FlySkyIBus.h"

//uint32_t _bytes_rxed = 0;
//uint8_t buffer[PROTOCOL_LENGTH];

/*
// RX interrupt handler
void on_uart_rx(void) {
//void on_uart_rx(void) {
    while (uart_is_readable(UART_ID)) {
        //uint8_t ch = uart_getc(UART_ID);
        uart_read_blocking(UART_ID, buffer, PROTOCOL_LENGTH);
        *bytes_rxed +=PROTOCOL_LENGTH;
    };
};
*/

void FSIBus_Init(int UART_RX_PIN, FSKY *fsky) 
{
    fsky->UART_ID = uart1;
    fsky->BAUD_RATE = 115200;
    fsky->DATA_BITS = 8;
    fsky->STOP_BITS  = 1;
    fsky->PARITY = UART_PARITY_NONE;
    fsky->PROTOCOL_LENGTH = hPROTOCOL_LENGTH;
    fsky->PROTOCOL_OVERHEAD = 3; // <len><cmd><data....><chkl><chkh>
    fsky->PROTOCOL_TIMEGAP  = 3; // Packets are received every ~7ms so use ~half that for the gap
    fsky->PROTOCOL_CHANNELS = hPROTOCOL_CHANNELS;
    fsky->bytes_rxed = 0;
    
    //fsky->PROTOCOL_COMMAND40 = hPROTOCOL_COMMAND40; // Command is always 0x40 ??
    //printf("%i\n", fsky->bytes_rxed);
    //printf("%i\n", fsky->BAUD_RATE);

    // Set up our UART with a basic baud rate.
    uart_init(fsky->UART_ID, fsky->BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    //gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(fsky->UART_ID, false, false);

    // Set our data format
    uart_set_format(fsky->UART_ID, fsky->DATA_BITS, fsky->STOP_BITS, fsky->PARITY);

    // Turn off FIFO's - we want to do this character by character
    //uart_set_fifo_enabled(UART_ID, false);

    // Turn ON? FIFO's - we want to read the whole buffer?
    uart_set_fifo_enabled(fsky->UART_ID, true);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    //int UART_IRQ = fsky->UART_ID == uart1 ? UART1_IRQ : UART0_IRQ;
    
    // And set up and enable the interrupt handlers
    //irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    //irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    //uart_set_irq_enables(fsky->UART_ID, true, false);
    //uart_set_irq_enables(UART_ID, false, false); // no interrupts until we know what it means

};

void FSIBus_Read(FSKY *fsky) 
{

    //fsky->stream = &stream; // FlySkyIBus.stream is a pointer to stream?
    fsky->state = DISCARD;
    fsky->ptr = 0;
    fsky->len = 0;
    fsky->chksum = 0;
    fsky->lchksum = 0; // we have to store the chksum low byte until we read the high byte
    fsky->MaxAttempts = 10;

    uint32_t last = time_us_32()/1000.;
    int attempts = 1;
    //uint8_t _PROTOCOL_COMMAND40 = fsky->PROTOCOL_COMMAND40;
    //printf("attempts, MaxAttempts: %i, %i\n", attempts, fsky->MaxAttempts);

    while (attempts < fsky->MaxAttempts)
    //while (uart_is_enabled(fsky->UART_ID)) // This while condition is wrong
    {
        uint32_t now = time_us_32()/1000.; /* Returns the number of milliseconds passed since the 
        pico booted*/
        if (now - last >= fsky->PROTOCOL_TIMEGAP)
        {
            fsky->state = GET_LENGTH;
        }
        last = now;

        // copied from arduino! :
        //uint8_t v = stream->read(); /* "to access members of a structure through a pointer, use the arrow operator"*/
        long v = uart_getc(fsky->UART_ID);
        //printf("v: %i\n", v);

        switch (fsky->state)
        {
            case GET_LENGTH:
                if (v <= fsky->PROTOCOL_LENGTH)
                {
                    fsky->ptr = 0;
                    fsky->len = v - fsky->PROTOCOL_OVERHEAD;
                    fsky->chksum = 0xFFFF - v;
                    fsky->state = GET_DATA;
                }
                else
                {
                    fsky->state = DISCARD;
                }
                break;

            case GET_DATA:
                fsky->buffer[fsky->ptr++] = v; 
                fsky->chksum -= v;
                fsky->bytes_rxed++;
            if (fsky->ptr == fsky->len)
            {
                fsky->state = GET_CHKSUML; 
            }
            break;
        
            case GET_CHKSUML: // CHECK-SUM Low Byte
                fsky->lchksum = v;
                fsky->state = GET_CHKSUMH; 
                break;

            case GET_CHKSUMH: // CHECK-SUM Hight Byte
                // Validate checksum
                if (fsky->chksum == (v << 8) + fsky->lchksum) /* << = left shifting an integer “x” with an 
                integer “y” denoted as ‘(x<<y)’ is equivalent to multiplying 
                x with 2^y (2 raised to power y).*/
                {
                // Execute command - we only know command 0x40
                    switch (fsky->buffer[0])
                    {
                        case PROTOCOL_COMMAND40:
                            // Valid - extract channel data
                            for (uint8_t i = 1; i < fsky->PROTOCOL_CHANNELS * 2 + 1; i += 2)
                            {
                                fsky->channel[i / 2] = fsky->buffer[i] | (fsky->buffer[i + 1] << 8);
                            }
                        break;

                        default:
                        break;
                    }
                }
                fsky->state = DISCARD;
                break;

            case DISCARD:
                default:
                    break;
        }
    }
    attempts++;

};

uint16_t FSIBus_readChannel(uint8_t channelNr, FSKY *fsky)
{
  if (channelNr < fsky->PROTOCOL_CHANNELS)
  {
    return fsky->channel[channelNr];
  }
  else
  {
    return 0;
  }
};