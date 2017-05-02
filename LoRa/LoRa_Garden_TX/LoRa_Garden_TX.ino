

#include <SPI.h>
#include <Crypto.h>
#include <AES.h>
#include <RH_RF95.h>
// oled display
#include "U8glib.h"
#include "config.h"
#include "GardenPayload.h"
#include "AckPayload.h"

#define ENCRYPTION_BUFFER_SIZE 16

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

#define RF95_FREQ 868.0

#define PIN_SENSOR_SOLAR_CURRENT A0
#define PIN_SENSOR_SOLAR_VOLTS A1
#define PIN_SENSOR_SOIL A2
#define PIN_SENSOR_LIGHT A3

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// OLED
U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);

theapi::GardenPayload tx_payload = theapi::GardenPayload();
theapi::AckPayload ack_payload = theapi::AckPayload();

uint8_t msg_id = 0;
int rssi = 0;

typedef struct {
  int sent;
  int got;
  int rssi;
}
monitor_t;
monitor_t monitor;

AES128 cipher;
uint8_t encrypted_buffer[ENCRYPTION_BUFFER_SIZE];
uint8_t decrypted_buf[ENCRYPTION_BUFFER_SIZE];

uint8_t rx_node_id = 1;
uint8_t garden_node_id = 2;

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);

  //u8g.setRot180();
  monitor.sent = 0;
  monitor.got = 0;
  monitor.rssi = 0;
  displayUpdate();
  
  delay(100);

  //Serial.println("LoRa Garden TX");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("init failed");
    while (1);
  }
  Serial.println("init OK");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  //Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  
  rf95.setTxPower(10, false);
}


void loop() {

  tx_payload.setMsgId(++msg_id);
  // Internal 1.1v aref
  uint16_t vcc = readVcc();
  tx_payload.setVcc(vcc);
  tx_payload.setChargeMa(readSolarCurrent());

  // Vcc aref
  tx_payload.setChargeMv(readSolarVolts(vcc));
  tx_payload.setLight(readLight(vcc));
  tx_payload.setSoil(readSoil(vcc));
  
  tx_payload.setTemperature(readTemperature());
  
  uint8_t payload_buf[tx_payload.size()];
  tx_payload.serialize(payload_buf);
  //rf95.send(payload_buf, tx_payload.size());
  cipher.encryptBlock(encrypted_buffer, payload_buf);
  //rf95.setHeaderTo(rx_node_id);
  //rf95.setHeaderFrom(garden_node_id);
  rf95.send(encrypted_buffer, ENCRYPTION_BUFFER_SIZE);

  
  int got = 0;
  monitor.sent = msg_id;
  monitor.got = got;
  monitor.rssi = rssi;
  displayUpdate();

  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(2000)) { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len)) {
      RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.print("headerFrom: "); Serial.println(rf95.headerFrom());
      //Serial.print("Got reply: ");
      //Serial.println((char*)buf);
      //got = atoi((char*)buf);
      //Serial.println(got);
      Serial.print("RSSI: ");
      rssi = rf95.lastRssi();
      Serial.println(rssi, DEC);    

      //uint8_t ack_payload_buf[ack_payload.size()];
      uint8_t decrypted_ack_buf[16];
      cipher.decryptBlock(decrypted_ack_buf, buf);
      ack_payload.unserialize(decrypted_ack_buf);

      got = ack_payload.getMsgId();
      Serial.print("msg_id: ");
      Serial.println(got, DEC);

      monitor.got = got;
      monitor.rssi = rssi;
      displayUpdate();
    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply");
  }

  
  delay(5000);
}

void displayUpdate()
{
  // picture loop
  u8g.firstPage();
  do {
    draw();
  } while( u8g.nextPage() );
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here

  u8g.setFont(u8g_font_fub11n);
  //u8g.setFont(u8g_font_unifont);
  u8g.setFontPosTop();
  
  u8g.setPrintPos(0, 0);
  u8g.print(monitor.sent);

  u8g.setPrintPos(60, 0);
  u8g.print(monitor.got);
  
  u8g.setPrintPos(0, 20);
  u8g.print(monitor.rssi);
  
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
  analogReference(INTERNAL);
  uint16_t val;
  // After changing the analog reference, 
  // the first few readings from analogRead() may not be accurate.
  for (uint8_t i = 0; i < 3; i++) { 
    val = analogRead(PIN_SENSOR_SOLAR_CURRENT);
  }
  // Average the next readings.
  val = 0;
  for (uint8_t i = 0; i < 2; i++) { 
    val += analogRead(PIN_SENSOR_SOLAR_CURRENT);
  }

  return val / 2;
}

uint16_t readSolarVolts(uint16_t vcc) {
  // Voltage divider to half the incoming volts.
  analogReference(DEFAULT);
  uint16_t val;
  // After changing the analog reference, 
  // the first few readings from analogRead() may not be accurate.
  for (uint8_t i = 0; i < 2; i++) { 
    val = analogRead(PIN_SENSOR_SOLAR_VOLTS);
  }
  // Add the next 2 readings.
  // To avergage and account for the halving votage divider.
  val = 0;
  for (uint8_t i = 0; i < 2; i++) { 
    val += analogRead(PIN_SENSOR_SOLAR_VOLTS);
  }

  return vcc / 1024.0 * val;
}

uint16_t readLight(uint16_t vcc) {
  analogReference(DEFAULT);
  uint16_t val;
  // After changing the analog reference, 
  // the first few readings from analogRead() may not be accurate.
  for (uint8_t i = 0; i < 2; i++) { 
    val = analogRead(PIN_SENSOR_LIGHT);
  }
  // Average the next 2 readings.
  val = 0;
  for (uint8_t i = 0; i < 2; i++) { 
    val += analogRead(PIN_SENSOR_LIGHT);
  }

  return val / 2;
}

uint16_t readSoil(uint16_t vcc) {
  analogReference(DEFAULT);
  uint16_t val;
  // After changing the analog reference, 
  // the first few readings from analogRead() may not be accurate.
  for (uint8_t i = 0; i < 2; i++) { 
    val = analogRead(PIN_SENSOR_SOIL);
  }
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

