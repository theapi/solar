/*
  Digital Pot Control

*/


// inslude the SPI library:
#include <SPI.h>


// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
const int address = 0;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
  
  //digitalPotWrite(address, 128);
}

void loop() {
  
  
    // change the resistance on this channel from min to max:
    for (int level = 0; level <= 128; level++) {
      digitalPotWrite(address, level);
      delay(50);
    }
    // wait a second at the top:
    delay(5000);
    // change the resistance on this channel from max to min:
    for (int level = 128; level >= 0; level--) {
      digitalPotWrite(address, level);
      delay(50);
    }
    
}

void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 
}
