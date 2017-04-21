

#include <SPI.h>
#include <RH_RF95.h>
// oled display
#include "U8glib.h"
#include "GardenPayload.h"

#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

#define RF95_FREQ 868.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// OLED
U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);

theapi::GardenPayload tx_payload = theapi::GardenPayload();

uint8_t msg_id = 0;

typedef struct {
  int sent;
  int got;
  int rssi;
}
monitor_t;
monitor_t monitor;

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

  Serial.println("LoRa GArden TX");

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
  
  rf95.setTxPower(23, false);
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
  rf95.send(payload_buf, tx_payload.size());

  int rssi = 0;
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
    } else {
      Serial.println("Receive failed");
    }
  } else {
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


