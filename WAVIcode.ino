// Buzzer
#define trigPin 9
#define echoPin 10
#define buzzPin 3
#define switchPin 2
int switchMode = 0;

// Bluetooth
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(switchPin, OUTPUT);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

//  /* Wait for connection */
//  while (! ble.isConnected()) {
//      delay(500);
//  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }  
}

void loop() {
  // put your main code here, to run repeatedly:
  long duration, distance;
  switchMode = digitalRead(switchPin);
  Serial.println(switchMode);
  if (switchMode == HIGH)
  {
    Serial.println("Bluetooth Mode");
    // Check for user input
    char inputs[BUFSIZE+1];
    if ( getUserInput(inputs, BUFSIZE) )
    {
      // Send characters to Bluefruit
      Serial.print("[Send] ");
      Serial.println(inputs);
  
      ble.print("AT+BLEUARTTX=");
      ble.println(inputs);
  
      // check response stastus
      if (! ble.waitForOK() ) {
        Serial.println(F("Failed to send?"));
      }
    }
  }
  else if (switchMode == LOW)
  {
    Serial.println("Buzzer Mode");
    digitalWrite(trigPin, LOW);  // Added this line
    //delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    //delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    
    Serial.print(distance);
    Serial.println(" cm");
    
    //short range: 70 cm
    if(distance > 20 && distance<70){
    tone(buzzPin, 1500, 200);    
    } 
    else if(distance > 70 && distance < 150){
    tone(buzzPin, 1300, 200);
    }
    else if(distance > 150 && distance < 300){
    tone(buzzPin,1000, 200);
    }
    //delay(500);    
 }
 else
 {
  Serial.println("Error mode");
 }
}

bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (!Serial.available()) && !timeout.expired() ) { delay(1); }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && (Serial.available()) );
  return true;
}
