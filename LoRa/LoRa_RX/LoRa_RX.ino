/**
 * Listen for LoRa transmissions,
 * then pass the data through Serial to be processed by the monitor.
 */


#include <SPI.h>
//#include <Crypto.h>
//#include <AES.h>
#include <RH_RF95.h>

#include "config.h"
#include "Payload.h"
#include "GardenPayload.h"
#include "AckPayload.h"
#include "SignalPayload.h"


//#define ENCRYPTION_BUFFER_SIZE 16

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

// UK frequency
#define RF95_FREQ 868.0

// Blinky on receipt
#define LED 13

// Shift register pins for the LEDs on the front panel.
#define SR_LATCH 16
#define SR_CLK 17
#define SR_DATA 15
  
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);


theapi::GardenPayload garden_payload = theapi::GardenPayload();
theapi::AckPayload ack_payload = theapi::AckPayload();
theapi::SignalPayload signal_payload = theapi::SignalPayload();

//AES128 cipher;
//uint8_t encrypted_buffer[ENCRYPTION_BUFFER_SIZE];
//uint8_t decrypted_buf[ENCRYPTION_BUFFER_SIZE];

uint8_t rx_node_id = 1;
uint8_t garden_node_id = 2;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_CLK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);

  // Show the led panel is working.
  garden_payload.setMsgId(0b10101010);
  updateLedPanel();

  Serial.println("LoRa RX");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);

  rf95.setTxPower(23, false);
}

void loop() {
  if (rf95.available()) {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len)) {
      digitalWrite(LED, HIGH);
      Serial.println();
      RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.print("headerFrom: "); Serial.println(rf95.headerFrom());
//      Serial.print("Got: ");
//      Serial.println((char*)buf);
      
      Serial.print("RSSI: ");
      int rssi = rf95.lastRssi();
      Serial.println(rf95.lastRssi(), DEC);

      // Decrypt the message.
      //cipher.decryptBlock(decrypted_buf, buf);
      //garden_payload.unserialize(decrypted_buf);
      garden_payload.unserialize(buf);
      Serial.print("msg_id: ");
      Serial.println(garden_payload.getMsgId());

      // Send the garden data to the monitor.
      uint8_t payload_buf[garden_payload.size()];
      garden_payload.serialize(payload_buf);
      Serial.write('\t'); // Payload start byte
      Serial.write(payload_buf, garden_payload.size());

      // Send radio signal data to the monitor.
      signal_payload.setMsgId(garden_payload.getMsgId());
      signal_payload.setRssi(rssi);
      signal_payload.setSnr(rf95.lastSNR());
      signal_payload.setFreqError(rf95.frequencyError());
      uint8_t signal_payload_buf[signal_payload.size()];
      signal_payload.serialize(signal_payload_buf);
      Serial.write('\t'); // Payload start byte
      Serial.write(signal_payload_buf, signal_payload.size());

      updateLedPanel();
      
      // There needs to be a small delay before sending the reply.
      // YUK
      delay(100);

      // Send a reply
      ack_payload.setMsgId(garden_payload.getMsgId());
      //ack_payload.setValue(1234);
      uint8_t ack_payload_buf[ack_payload.size()];
      ack_payload.serialize(ack_payload_buf);
      rf95.send(ack_payload_buf, ack_payload.size());
      //uint8_t encrypted_ack_buffer[16];
      //cipher.encryptBlock(encrypted_ack_buffer, ack_payload_buf);
      //rf95.setHeaderTo(garden_node_id);
      //rf95.setHeaderFrom(rx_node_id);
      //rf95.send(encrypted_ack_buffer, 16);
      rf95.waitPacketSent();

      
      digitalWrite(LED, LOW);
      
    } else {
      Serial.println("Receive failed");
    }
  }
}


void updateLedPanel() {
  byte d = garden_payload.getMsgId();
  
  digitalWrite(SR_LATCH, LOW);
  // Bit shift left as there is no 8th led.
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, d << 1);
  digitalWrite(SR_LATCH, HIGH);
}



