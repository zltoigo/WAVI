#!/usr/bin/env python3
'''
Modified version of jpegstream.py

'''

import time
import serial
import cv2
from sys import stdout

from helpers import *

# Modifiable params --------------------------------------------------------------------

PORT = 'COM2'         # Windows

BAUD = 115200       # Change to 115200 for Due

# main ------------------------------------------------------------------------------

if __name__ == '__main__':

    # Open connection to Arduino with a timeout of two seconds
    port = serial.Serial(PORT, BAUD, timeout=2)

    # Report acknowledgment from camera
    getack(port)

    # Wait a spell
    time.sleep(0.2)

    # Send start flag
    sendbyte(port, 1)
    
    i = 0
    # Loop over bytes from Arduino for a single image
    while True:
        
        tmpfile = open("tmp" + str(i) + ".jpg", "wb")
        #tmpfile = open("tmp.jpg", "wb")

        # Loop over bytes from Arduino for a single image
        written = False
        prevbyte = None

        while True:

            # Read a byte from Arduino
            currbyte = port.read(1)

            # If we've already read one byte, we can check pairs of bytes
            if prevbyte:

                # Start-of-image sentinel bytes: write previous byte to temp file
                if ord(currbyte) == 0xd8 and ord(prevbyte) == 0xff:
                    tmpfile.write(prevbyte)
                    written = True

                # Inside image, write current byte to file
                if written:
                    tmpfile.write(currbyte)

                # End-of-image sentinel bytes: close temp file and display its contents
                if ord(currbyte) == 0xd9 and ord(prevbyte) == 0xff:
                    tmpfile.close()
                    img = cv2.imread("tmp" + str(i) + ".jpg")
                    #img = cv2.imread("tmp.jpg")
                    print("Picture taken!")
                    break  

            # Track previous byte
            prevbyte = currbyte
        
        i = i + 1

    # Send stop flag
    sendbyte(port, 0)
