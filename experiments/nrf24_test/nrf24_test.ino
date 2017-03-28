/**
  * Testing the nrf24 setup.
  */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include <Nrf24Payload.h>



#define DEVICE_ID 'D'
 
//#define RX_ADDRESS "AAAAA"
//#define RX_ADDRESS "BBBBB"
//#define RX_ADDRESS "CCCCC"
#define RX_ADDRESS "DDDDD"
//#define RX_ADDRESS "EEEEE"


#define BASE_ADDRESS "2BASE"

#define MESSAGE_LEN 40

#define PIN_CE  7
#define PIN_CSN 8


byte addresses[][6] = {"1Node","2Node"};


byte role = 1; // 0 is receiver
unsigned long id = 0;


RF24 radio(PIN_CE, PIN_CSN);


void setup() {
  Serial.begin(115200);
  printf_begin();

  // Setup and configure nrf24 radio
  radio.begin(); // Start up the radio
  radio.setChannel(2);
  radio.setPayloadSize(Nrf24Payload_SIZE);               
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(3,15); //  delay between retries & number of retries

  

  Serial.println("Setup");

  Serial.print("Size of payload = ");
  Serial.println(radio.getPayloadSize());

  // Open a writing and reading pipe on each radio, with opposite addresses
  if (role == 1){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  } else {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }

  // Start the radio listening for data
  radio.startListening();

    // Dump the configuration of the rf unit for debugging
  radio.printDetails();
}


void loop() {

  if (role == 1) {
  
      
      radio.stopListening(); 

      ++id;
      Serial.print("Sending: ");
      Serial.println(id);
      
      Nrf24Payload payload = Nrf24Payload();
      payload.setDeviceId(DEVICE_ID);
      payload.setId(id);
      payload.setType('S');
      // Forward the message through the nrf24 radio
      uint8_t tx[Nrf24Payload_SIZE];
      payload.serialize(tx);
      
      radio.write( &tx, Nrf24Payload_SIZE);
      radio.startListening();   
  
      delay(1000); 
  } else {

    // Handle any messages from the radio
    while (radio.available()) {
      // Create a byte array to hold the raw bytes from the radio.
      uint8_t rx[Nrf24Payload_SIZE];
      // Read from the radio into the byte array.
      radio.read(&rx, Nrf24Payload_SIZE);
      // Create a payload object to handle the radio data.
      Nrf24Payload rx_payload = Nrf24Payload();
      // Convert the raw byte array into useful data.
      rx_payload.unserialize(rx);

      Serial.print("Got: ");
      Serial.println(rx_payload.getId());

    }

  
    
  }

}

