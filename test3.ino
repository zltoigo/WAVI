/*
JPEGStream.ino : capture images and stream them to host as JPEG files
Copyright (C) Simon D. Levy 2017
This file is part of BreezyArduCAM.
BreezyArduCAM is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
BreezyArduCAM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with BreezyArduCAM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include <BreezyArduCAM.h>
#include <SPI.h>

static const int CS = 7;
int switchPin = 4;
int oldSwitchState = HIGH;

Serial_ArduCAM_FrameGrabber fg;

ArduCAM_Mini_2MP myCam(CS, &fg);

void setup(void) 
{
    pinMode(switchPin, INPUT_PULLUP);
    // ArduCAM Mini uses both I^2C and SPI buses
    Wire.begin();
    SPI.begin();
    
    // Fastest baud rate (change to 115200 for Due)
    Serial.begin(115200);    
    // Start the camera in JPEG mode with a specific image size
    myCam.beginJpeg1600x1200();
}

void loop(void) 
{
    byte switchState = digitalRead(switchPin);
    
    if (switchState != oldSwitchState)
    {
      oldSwitchState = switchState;
      if (switchState == HIGH)
      {
        myCam.capture();   
      }
    }
}
