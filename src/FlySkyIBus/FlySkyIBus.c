/*
 * Simple interface to the Fly Sky IBus RC system.
 * inspired on: https://gitlab.com/timwilkinson/FlySkyIBus
 * Translated to C and adapted to PICO sdk (by a complete C/C++ newbie...).
 * 
 * Author: R. Azzollini (2022)
 */

#include "FlySkyIBus.h"


static FSKY *fsky;

//void FSIBus_Read(FSKY *fsky);
//void FSIBus_Listen(FSKY *fsky);



void FSIBus_Init(int UART_RX_PIN, FSKY *fsky) 
{
    fsky->UART_ID = uart1;
    fsky->BAUD_RATE = 115200;
    fsky->DATA_BITS = 8;
    fsky->STOP_BITS  = 1;
    fsky->PARITY = UART_PARITY_NONE;
    fsky->PROTOCOL_LENGTH = hPROTOCOL_LENGTH;
    fsky->PROTOCOL_OVERHEAD = hPROTOCOL_OVERHEAD; // <len><cmd><data....><chkl><chkh>
    fsky->PROTOCOL_TIMEGAP  = 3; // Packets are received every ~7ms so use ~half that for the gap
    fsky->PROTOCOL_CHANNELS = hPROTOCOL_CHANNELS;
    fsky->PROTOCOL_COMMAND40 = hPROTOCOL_COMMAND40;
    fsky->bytes_rxed = 0;
    fsky->attempts = 0;
    fsky->MaxAttempts = 10;

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

    // Turn off FIFO's - I think this is not what we want
    //uart_set_fifo_enabled(fsky->UART_ID, false);

    // Turn ON? FIFO's - seems like this is what I want...
    uart_set_fifo_enabled(fsky->UART_ID, true);
};


void FSIBus_Read_thenParse(FSKY *fsky) 
{

    fsky->ptr = 0;
    fsky->len = 0;
    fsky->chksum = 0;
    fsky->attempts = 0;
    fsky->bytes_rxed = 0;

    int NomDataLength = fsky->PROTOCOL_LENGTH-fsky->PROTOCOL_OVERHEAD;

    while (fsky->attempts < fsky->MaxAttempts && fsky->bytes_rxed!=NomDataLength)
    {
        fsky->attempts++;
        fsky->ptr = 0;
        fsky->bytes_rxed = 0;

        // initializing the buffer
        for (int i=0; i<NomDataLength; i++)
            fsky->buffer[i] = 0;

        // initializing the channels
        for (int i=0; i<fsky->PROTOCOL_CHANNELS; i++)
            fsky->channel[i] = 0;

        uint8_t v;  
        int rawptr = 0; // index within the raw buffer
        uint8_t rawbuffer[fsky->PROTOCOL_LENGTH];   // raw buffer holds all the bytes in PROTOCOL_LENGTH, 
                                                    // then we parse it

        for (int i=0; i<fsky->PROTOCOL_LENGTH; i++)
            rawbuffer[i] = 0;

        int gapus = 1000 * fsky->PROTOCOL_TIMEGAP;

        while (uart_is_readable_within_us(fsky->UART_ID, gapus) && rawptr<fsky->PROTOCOL_LENGTH)
        //while (uart_is_readable(fsky->UART_ID) && rawptr<fsky->PROTOCOL_LENGTH)
        {
            v = uart_getc(fsky->UART_ID);
            rawbuffer[rawptr++] = v;
        }

        // Parsing begins
        
        if ((rawbuffer[0] != fsky->PROTOCOL_LENGTH) || (rawbuffer[1] != fsky->PROTOCOL_COMMAND40))
        {
            //printf("First byte is wrong: %d\n", rawbuffer[0]);
            continue;
        } 

        rawptr = 0;
        fsky->ptr = 0;

        v = rawbuffer[0];

        fsky->len = v - fsky->PROTOCOL_OVERHEAD;
        fsky->chksum = 0xFFFF - v;

        for (int i=1; i<NomDataLength+1; i++)
        {
            v = rawbuffer[i];
            fsky->buffer[fsky->ptr++] = v;
            fsky->chksum -= v;
            fsky->bytes_rxed++;
        }

        if ((fsky->chksum == (rawbuffer[NomDataLength+1] | rawbuffer[NomDataLength+2]<<8)) &&\
            (fsky->buffer[0] == fsky->PROTOCOL_COMMAND40))
        {
            for (uint8_t i = 1; i < fsky->PROTOCOL_CHANNELS * 2 + 1; i += 2)
                fsky->channel[i / 2] = fsky->buffer[i] | (fsky->buffer[i + 1] << 8);
        } else
        {
            fsky->bytes_rxed = 0;
        }
    }
};

int16_t FSIBus_readNormChannel(uint8_t channelNr, FSKY *fsky)
{
  
  uint16_t value;
  float bias, scale;

  switch (channelNr)
  {
    case FSthrust:
        value = fsky->channel[FSthrust];
        bias = 1000;
        scale = 1/10.;
        break;
    case FSyaw:
        value = fsky->channel[FSyaw];
        bias = 1500;
        scale = 1/5.;
        break;
    case FSpitch:
        value = fsky->channel[FSpitch];
        bias = 1500;
        scale = 1/5.;
        break;
    case FSroll:
        value = fsky->channel[FSroll];
        bias = 1500;
        scale = 1/5.;
        break;
    case FSdileft:
        value = fsky->channel[FSdileft];
        bias = 1000;
        scale = 1/10.;
        break;
    case FSdiright:
        value = fsky->channel[FSdiright];
        bias = 1000;
        scale = 1/10.;
        break;
    default:
        value = 1;
        bias = 0;
        scale = 0;
        break;
  }
  return ((value-bias)*scale);
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