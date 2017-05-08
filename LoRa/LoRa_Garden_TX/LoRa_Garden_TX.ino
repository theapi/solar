

#include <SPI.h>
#include <Wire.h>
#include <Crypto.h>
#include <AES.h>
#include <RH_RF95.h>
#include <Adafruit_ADS1015.h>

#include "config.h"
#include "GardenPayload.h"
#include "AckPayload.h"

#define ENCRYPTION_BUFFER_SIZE 16

#define RFM95_CS 10
//#define RFM95_RST 2
#define RFM95_INT 3

#define RF95_FREQ 868.0

#define PIN_SENSOR_LIGHT A0
#define PIN_SENSOR_SOLAR_VOLTS A1
#define PIN_SENSOR_SOIL A2


// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// The external I2C ADC.
Adafruit_ADS1015 ads;


theapi::GardenPayload tx_payload = theapi::GardenPayload();
theapi::AckPayload ack_payload = theapi::AckPayload();

uint8_t msg_id = 0;
int rssi = 0;

AES128 cipher;
uint8_t encrypted_buffer[ENCRYPTION_BUFFER_SIZE];
uint8_t decrypted_buf[ENCRYPTION_BUFFER_SIZE];

void setup() {
  Serial.begin(115200);

  ads.setGain(GAIN_TWO); // 2x gain   +/- 2.048V  1 bit = 1mV
  
  delay(100);

  while (!rf95.init()) {
    Serial.println("init failed");
    while (1);
  }
  Serial.println("init OK");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  rf95.setTxPower(10, false);

  ads.begin();
}

void loop() {

  tx_payload.setMsgId(++msg_id);

  uint16_t vcc = readVcc();
  tx_payload.setVcc(vcc);

  // Read the temperature while the aref settles.
  tx_payload.setTemperature(readTemperature());
  
  tx_payload.setChargeMa(readSolarCurrent());
  tx_payload.setChargeMv(readSolarVolts());
  tx_payload.setLight(readLight(vcc));
  tx_payload.setSoil(readSoil(vcc));
  
  // Build the payload buffer.
  uint8_t payload_buf[tx_payload.size()];
  tx_payload.serialize(payload_buf);
  // Encrypt the payload.
  cipher.encryptBlock(encrypted_buffer, payload_buf);
  // Send it.
  rf95.send(encrypted_buffer, ENCRYPTION_BUFFER_SIZE);
  rf95.waitPacketSent();
  
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(2000)) { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len)) {
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("RSSI: ");
      rssi = rf95.lastRssi();
      Serial.println(rssi, DEC);    

      uint8_t decrypted_ack_buf[16];
      cipher.decryptBlock(decrypted_ack_buf, buf);
      ack_payload.unserialize(decrypted_ack_buf);

    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply");
  }
  
  delay(5000);
}

/**
 * Read the internal voltage.
 */
uint16_t readVcc() 
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  _delay_ms(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  uint16_t result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

uint16_t readSolarCurrent() {
  // 1 ohm inline resistor
  return ads.readADC_Differential_0_1();
}

uint16_t readSolarVolts() {
  // Voltage divider to reduce the maximum to 2V.
  // 8V -> 2V
  // Solar Panel --- 300K --- | --- 100K --- GND
  return ads.readADC_SingleEnded(2);
}

uint16_t readLight(uint16_t vcc) {
  // Can't use a simple LDR as it will use 4.5mA
  // @link https://learn.adafruit.com/photocells/using-a-photocell
  // Maybe a lux meter - https://www.adafruit.com/product/439
  
  return 0;
}

uint16_t readSoil(uint16_t vcc) {
  uint16_t val;

  // This first reading after a mux change is unreliable.
  val = analogRead(PIN_SENSOR_SOIL);

  // Average the next 2 readings.
  val = 0;
  for (uint8_t i = 0; i < 2; i++) { 
    val += analogRead(PIN_SENSOR_SOIL);
  }

  return val / 2;
}

uint16_t readTemperature() {
return 25; //@todo do a real reading

}

