/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#define trigPin 9
#define echoPin 10
#define buzzPin 3

// IR Emitter 5

// IR Sensor 6

//SCLK  13 - serial clock pin, connected to SCK on Arduino or MCU
//MISO 12 - Master In Slave Out SPI pin (nRF51 -> Arduino communication)
//MOSI 11 - Master Out Slave In SPI pin (Arduino -> nRF51 communication)
//CS 8 - Chip Select SPI pin, which is used to indicate that the SPI device is currently in use.
//IRQ 7 - RF51 -> Arduino 'interrupt' pin that lets the Arduino or MCU know when data is available on the nRF51...
//i...ndicating that a new SPI transaction should be initiated by the Arduino/MCU.

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzPin, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
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
  delay(500);
}
