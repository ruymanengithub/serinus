#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""

pseudo-code for an ibus reader.
following on the heels of:
    http://www.penguintutor.com/news/electronics/rc-pico-ibus#google_vignette


# returns list of channel values. First value (pseudo channel 0) is status
# 0 = initial values
# 1 = new values
# -1 = failed to receive data old values sent
# -2 = checksum error


Created on Wed Jun  1 20:38:14 2022


@author: raf
"""

from machine import UART

class IBus ():
    
    # Number of channels (FS-RX2A has 10?)
    def __init__ (self, uart_num, baud=115200, num_channels=10):
        self.uart_num = uart_num 
        self.baud = baud
        self.uart = UART(self.uart_num, self.baud)
        self.num_channels = num_channels
        self.Nattempts = 10
        self.rxbuf = 32
        # ch is channel value
        self.ch = []
        # Set channel values to 0
        for i in range (self.num_channels+1):
            self.ch.append(0)
            
            
    # Returns list with raw data
    def read(self):
        # Max Nattempts to read
        for z in range(self.Nattempts):
            buffer = bytearray (self.rxbuf-1)
            char = self.uart.read(1) # reads one byte
            # check for 0x20
            if char == b'\x20':
                # read rest of string into buffer (bytes 1 to self.rxbuf, Nbytes=self.rxbuf-1)
                self.uart.readinto(buffer)
                checksum = 0xffdf # 0xffff - 0x20
                # check checksum 
                for i in range(self.rxbuf-3):
                    checksum -= buffer[i]
                # last two bytes in buffer hold the checksum value
                if checksum == (buffer[self.rxbuf-3] | buffer[self.rxbuf-2] << 8):
                    # buffer[0] = 0x40
                    self.ch[0] = 1 # status 1 = success
                    for i in range (1, self.num_channels + 1):
                        self.ch[i] = (buffer[(i*2)-1] + (buffer[i*2] << 8))                    
                    return self.ch
                else:
                    # Checksum error
                    self.ch[0] = -2
            else:
                self.ch[0] = -1
                
        # Reach here then timed out
        self.ch[0] = -1
        return self.ch
    
    
    # Convert to meaningful values - eg. -100 to 100
    # Typical use for FS-iA6B
    # channel 1 to 4 use type="default" provides result from -100 to +100 (0 in centre)
    # channel 5 & 6 are dials type="dial" provides result from 0 to 100 
    # Note approx depends upon calibration etc.
    @staticmethod
    def normalize (value, type="default"):
        if (type == "dial"):
            return ((value - 1000) / 10)
        else:
            return ((value - 1500) / 5)