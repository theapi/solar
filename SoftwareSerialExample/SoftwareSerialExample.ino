/*
  Software serial ATTINY85 test - requires 8mhz
 
 */
#include <SoftwareSerial.h>

byte num = 0;
byte debug_pin = 0; 
 
SoftwareSerial mySerial(4, 3); // RX, TX

void setup()  
{

  pinMode(debug_pin, OUTPUT);
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Setup");
}

void loop() // run over and over
{
  analogWrite(debug_pin, num); 
  mySerial.write(num);
  num++;

  delay(50);
  
  if (num ==255) {
    num = 0;
    delay(2000);
  }
}

