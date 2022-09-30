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

static FSKY *fsky;

void FSIBus_Read(FSKY *fsky);
void FSIBus_Listen(FSKY *fsky);

// RX interrupt handler
void on_uart_rx(void) {
    while (uart_is_readable(fsky->UART_ID)) {
        fsky->attempts++;
        printf("\nattempt: %d", fsky->attempts);
        //uint8_t ch = uart_getc(UART_ID);
        uint8_t buffer[hPROTOCOL_LENGTH];
        uart_read_blocking(fsky->UART_ID, buffer, hPROTOCOL_LENGTH);
        fsky->bytes_rxed += hPROTOCOL_LENGTH;
        //FSIBus_Read(fsky);
    };
};


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


/*

void FSIBus_Listen(FSKY *fsky) 
{

    uint8_t buffer;

    uart_read_blocking(fsky->UART_ID, &buffer, hPROTOCOL_LENGTH);
    fsky->bytes_rxed += hPROTOCOL_LENGTH;

    //fsky->stream = &stream; // FlySkyIBus.stream is a pointer to stream?
    fsky->state = DISCARD;
    fsky->ptr = 0;
    fsky->len = 0;
    fsky->chksum = 0;
    fsky->lchksum = 0; // we have to store the chksum low byte until we read the high byte
    //fsky->MaxAttempts = 10;
    fsky->attempts++;
    
    
};

*/

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

/*

void FSIBus_Read_thenParse_v0(FSKY *fsky) 
{

    fsky->state = DISCARD;
    fsky->ptr = 0;
    fsky->len = 0;
    fsky->chksum = 0;
    fsky->lchksum = 0; // we have to store the chksum low byte until we read the high byte
    fsky->attempts = 0;
    //fsky->uartReadable = 0;
    fsky->bytes_rxed = 0;

    int NomDataLength = fsky->PROTOCOL_LENGTH-fsky->PROTOCOL_OVERHEAD;

    while (fsky->attempts < fsky->MaxAttempts && fsky->bytes_rxed!=NomDataLength)
    {
        fsky->attempts++;
        fsky->ptr = 0;
        fsky->bytes_rxed = 0;

        for (int i=0; i<NomDataLength; i++)
            fsky->buffer[i] = 0;

        for (int i=0; i<fsky->PROTOCOL_CHANNELS; i++)
            fsky->channel[i] = 0;


        //fsky->state = GET_LENGTH;  
        uint8_t v;  
        int rawptr = 0; // index within the raw buffer
        uint8_t rawbuffer[fsky->PROTOCOL_LENGTH]; // raw buffer holds all the bytes in PROTOCOL_LENGTH, 
                                                // then we parse it

        for (int i=0; i<fsky->PROTOCOL_LENGTH; i++)
            rawbuffer[i] = 0;


        while (uart_is_readable(fsky->UART_ID) && rawptr < fsky->PROTOCOL_LENGTH) 
        {
            v = uart_getc(fsky->UART_ID);
            rawbuffer[rawptr++] = v; 
            //fsky->bytes_rxed++;
        }

        
        if (rawbuffer[0] != fsky->PROTOCOL_LENGTH) 
        {
            //printf("First byte is wrong: %d\n", rawbuffer[0]);
            continue;
        } else
        {
            fsky->state = GET_LENGTH;
        }

        rawptr = 0;
        fsky->ptr = 0;

        while (rawptr < fsky->PROTOCOL_LENGTH)
        {
            v = rawbuffer[rawptr++];

            switch (fsky->state)
            {
                case GET_LENGTH:
                    fsky->ptr = 0;
                    fsky->len = v - fsky->PROTOCOL_OVERHEAD;
                    fsky->chksum = 0xFFFF - v;
                    fsky->state = GET_DATA;
                    break;

                case GET_DATA:
                    fsky->buffer[fsky->ptr++] = v; 
                    fsky->chksum -= v;
                    fsky->bytes_rxed++;
                    if (fsky->ptr == fsky->len)
                    {
                        fsky->state = GET_CHKSUML; 
                        //printf("fsky->ptr == fsky->len\n");
                    }
                    break;
        
                case GET_CHKSUML: // CHECK-SUM Low Byte
                    fsky->lchksum = v;
                    fsky->state = GET_CHKSUMH; 
                    break;

                case GET_CHKSUMH: // CHECK-SUM Hight Byte
                    // Validate checksum
                    if (fsky->chksum == (v << 8 | fsky->lchksum)) // adding low and hi check-sum bytes
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
                                printf("\ndid not find command40!\n");
                                break;
                        }
                    } else
                    {
                        
                        fsky->bytes_rxed = 0;
                        fsky->state = DISCARD;
                    }
                    break;

                case DISCARD:
                    break;
                default:
                    fsky->state = DISCARD;
                    break;
            }
        }



    }


};

*/

/*

void FSIBus_Read_whileParsing(FSKY *fsky) 
{

    //fsky->stream = &stream; // FlySkyIBus.stream is a pointer to stream?
    fsky->state = DISCARD;
    fsky->ptr = 0;
    fsky->len = 0;
    fsky->chksum = 0;
    fsky->lchksum = 0; // we have to store the chksum low byte until we read the high byte
    //fsky->MaxAttempts = 10;
    fsky->attempts = 0;
    //fsky->uartReadable = 0;
    fsky->bytes_rxed=0;

    int NomDataLength = fsky->PROTOCOL_LENGTH-fsky->PROTOCOL_OVERHEAD;

    while (fsky->attempts < fsky->MaxAttempts && fsky->bytes_rxed!=NomDataLength)
    {
        fsky->attempts++;
        fsky->ptr = 0;
        fsky->bytes_rxed = 0;

        fsky->state = GET_LENGTH;  
        uint8_t v;  

        for (int i=0; i<NomDataLength; i++)
            fsky->buffer[i] = 0;

        for (int i=0; i<fsky->PROTOCOL_CHANNELS; i++)
            fsky->channel[i] = 0;
 

        while (uart_is_readable_within_us(fsky->UART_ID, 0) && (fsky->state != DISCARD))
        {

            v = uart_getc(fsky->UART_ID); // reads one character

            //if (v != fsky->PROTOCOL_LENGTH)
            //{
            //    fsky->state = DISCARD;
            //} else
            //{
            //    fsky->ptr = 0;
            //    fsky->len = v - fsky->PROTOCOL_OVERHEAD;
            //    fsky->chksum = 0xFFFF - v;
            //    fsky->state = GET_DATA;
            //}


            switch (fsky->state)
            {
                case GET_LENGTH:
                    if (v == fsky->PROTOCOL_LENGTH)
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
                    if (fsky->chksum == (v << 8 | fsky->lchksum)) // adding low and hi check-sum bytes
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
                                //printf("\ndid not find command40!\n");
                                break;
                        }
                    }
                    fsky->state = DISCARD;
                    break;

                case DISCARD:
                    break;
                default:
                    fsky->state = DISCARD;
                    break;
            }
        }
    }


};

*/

/*


void FSIBus_Read_Old(FSKY *fsky) 
{

    //fsky->stream = &stream; // FlySkyIBus.stream is a pointer to stream?
    fsky->state = DISCARD;
    fsky->ptr = 0;
    fsky->len = 0;
    fsky->chksum = 0;
    fsky->lchksum = 0; // we have to store the chksum low byte until we read the high byte
    fsky->MaxAttempts = 10;
    fsky->attempts = 0;
    //fsky->uartReadable = 0;

    //char buffer[hPROTOCOL_LENGTH];
    fsky->bytes_rxed = 0;

    for (int i=0; i<fsky->PROTOCOL_LENGTH; i++)
        fsky->buffer[i] = 0;
    
    while (fsky->attempts < fsky->MaxAttempts && fsky->bytes_rxed==0)
    {
        fsky->attempts++;
        
        while (uart_is_readable(fsky->UART_ID) && fsky->ptr < hPROTOCOL_LENGTH) 
        {
            uint8_t v = uart_getc(fsky->UART_ID);
            fsky->state = GET_LENGTH;

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
                    if (fsky->chksum == (v << 8) + fsky->lchksum) 
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
    }

    printf("Buffer In\n");

};

*/


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