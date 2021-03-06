/**
 * Listen for LoRa transmissions,
 * then pass the data through Serial to be processed by the monitor.
 */


#include <SPI.h>
//#include <Crypto.h>
//#include <AES.h>
#include <RH_RF95.h>

//#include "config.h"
#include "Payload.h"
#include "GardenPayload.h"
#include "SolarPayload.h"

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
theapi::SolarPayload solar_payload = theapi::SolarPayload();

//AES128 cipher;
//uint8_t encrypted_buffer[ENCRYPTION_BUFFER_SIZE];
//uint8_t decrypted_buf[ENCRYPTION_BUFFER_SIZE];

uint8_t rx_node_id = 1;
uint8_t garden_node_id = 2;

uint8_t current_payload;

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
  //garden_payload.setMsgId(0);
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

    uint8_t garden_payload_buf[garden_payload.size()];
    uint8_t solar_payload_buf[solar_payload.size()];

    if (rf95.recv(buf, &len)) {
      //digitalWrite(LED, HIGH);

      Serial.println();
     // RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.print("headerFrom: "); Serial.println(rf95.headerFrom());
//      Serial.print("Got: ");
//      Serial.println((char*)buf);

      Serial.print("RSSI: ");
      int16_t rssi = rf95.lastRssi();
      Serial.println(rssi, DEC);
      Serial.print("SNR: ");
      int16_t lastSNR = rf95.lastSNR();
      Serial.println(lastSNR, DEC);
      Serial.print("FREQ ERROR: ");
      int16_t frequencyError = rf95.frequencyError();
      Serial.println(frequencyError, DEC);

      // Check the first byte for the payload type.
      current_payload = buf[0];
      switch (current_payload) {
        case theapi::Payload::GARDEN:
          // Decrypt the message.
          //cipher.decryptBlock(decrypted_buf, buf);
          //garden_payload.unserialize(decrypted_buf);
          garden_payload.unserialize(buf);
          Serial.print("msg_id: ");
          Serial.println(garden_payload.getMsgId());

          garden_payload.setRssi(rssi);
          garden_payload.setSnr(lastSNR);
          garden_payload.setFreqError(frequencyError);

          // Send the garden data to the monitor.
          garden_payload.serialize(garden_payload_buf);
          Serial.write('\t'); // Payload start byte
          Serial.write(garden_payload_buf, garden_payload.size());

          updateLedPanel();
        break;

        case theapi::Payload::SOLAR:
          // Send the solar data to the monitor.
          solar_payload.unserialize(buf);
          //solar_payload.serialize(solar_payload_buf);
          solar_payload.setRssi(rssi);
          solar_payload.setSnr(lastSNR);
          solar_payload.setFreqError(frequencyError);

//            Serial.print("SOLAR: ");
//            Serial.print(solar_payload.getMsgType()); Serial.print(", ");
//            Serial.print(solar_payload.getDeviceId()); Serial.print(", ");
//            Serial.print(solar_payload.getMsgId()); Serial.print(", ");
//            Serial.print(solar_payload.getVcc()); Serial.print(", ");
//            Serial.print(solar_payload.getChargeMv()); Serial.print(", ");
//            Serial.print(solar_payload.getChargeMa()); Serial.print(", ");
//            Serial.print(solar_payload.getLight()); Serial.print(", ");
//            Serial.print(solar_payload.getTemperature()); Serial.print(", ");
//            Serial.print(solar_payload.getCpuTemperature()); Serial.print(", ");
//            Serial.print(solar_payload.getRssi()); Serial.print(", ");
//            Serial.print(solar_payload.getSnr()); Serial.print(", ");
//            Serial.print(solar_payload.getFreqError()); Serial.print(", ");
//            Serial.println();

          solar_payload.serialize(solar_payload_buf);
          Serial.write('\t'); // Payload start byte
          Serial.write(solar_payload_buf, solar_payload.size());

        break;
      }

    } else {
      Serial.println("Receive failed");
    }
  }
}


void updateLedPanel() {
  byte d = garden_payload.getChargeMa();

  digitalWrite(SR_LATCH, LOW);
  // Bit shift left as there is no 8th led.
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, d << 1);
  digitalWrite(SR_LATCH, HIGH);
}



