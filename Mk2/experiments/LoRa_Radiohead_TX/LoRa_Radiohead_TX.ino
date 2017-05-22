// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h>
// oled display
#include "U8glib.h"

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// OLED
U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);

typedef struct{
  int sent;
  int got;
  int rssi;
}

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
  tx_payload.setVcc(1234);
  tx_payload.setChargeMv(5678);
  tx_payload.setChargeMa(72);
  tx_payload.setLight(69);
  tx_payload.setSoil(76);
  tx_payload.setTemperature(79);
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


monitor_t;
monitor_t monitor;

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);

  //u8g.setRot180();
  monitor.sent = 0;
  monitor.got = 0;
  monitor.rssi = 0;
  displayUpdate();
  
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

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

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 1;  // packet counter, we increment per xmission

void loop()
{
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  
  //char radiopacket[20] = "Hello World #      ";
  char radiopacket[30]   = "                   ";
  itoa(packetnum, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  //radiopacket[19] = 0;
  radiopacket[30] = 0;
  
  //Serial.println("Sending..."); //delay(10);
  rf95.send((uint8_t *)radiopacket, 20);

  int rssi = 0;
  int got = 0;
  monitor.sent = packetnum;
  monitor.got = got;
  monitor.rssi = rssi;
  displayUpdate();

  ++packetnum;

  //Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  

  //Serial.println("Waiting for reply..."); delay(10);
  if (rf95.waitAvailableTimeout(2000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      got = atoi((char*)buf);
      Serial.println(got);
      Serial.print("RSSI: ");
      rssi = rf95.lastRssi();
      Serial.println(rssi, DEC);    

      monitor.got = got;
      monitor.rssi = rssi;
      displayUpdate();
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
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


