/**
 * Listen for LoRa transmissions,
 * then pass the data through Serial to be processed by the monitor.
 */


#include <SPI.h>
#include <RH_RF95.h>
// oled display
#include "U8glib.h"
#include "Payload.h"
#include "GardenPayload.h"
#include "AckPayload.h"


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

// OLED
U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);

theapi::GardenPayload garden_payload = theapi::GardenPayload();
theapi::AckPayload ack_payload = theapi::AckPayload();

typedef struct{
  int num;
  int rssi;
}
monitor_t;
monitor_t monitor;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_CLK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);

  //u8g.setRot180();
  monitor.num = 0;
  monitor.rssi = 0;
  displayUpdate();

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
      Serial.print("Got: ");
      Serial.println((char*)buf);
      
      Serial.print("RSSI: ");
      int rssi = rf95.lastRssi();
      Serial.println(rf95.lastRssi(), DEC);

      garden_payload.unserialize(buf);
      Serial.print("msg_id: ");
      Serial.println(garden_payload.getMsgId());
      
      uint8_t payload_buf[garden_payload.size()];
      garden_payload.serialize(payload_buf);
      Serial.write('\t'); // Payload start byte
      Serial.write(payload_buf, garden_payload.size());

      monitor.num = garden_payload.getMsgId();
      monitor.rssi = rssi;

      // Update the display now as there needs to be 
      // a small delay before sending the reply.
      displayUpdate();
      updateLedPanel();
      
      // Send a reply
      ack_payload.setMsgId(garden_payload.getMsgId());
      //ack_payload.setValue(1234);
      uint8_t ack_payload_buf[ack_payload.size()];
      ack_payload.serialize(ack_payload_buf);
      rf95.send(ack_payload_buf, ack_payload.size());
      rf95.waitPacketSent();

      
      digitalWrite(LED, LOW);
      
    } else {
      Serial.println("Receive failed");
    }
  }
}

void displayUpdate() {
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
  u8g.print(monitor.num);
  
  u8g.setPrintPos(0, 20);
  u8g.print(monitor.rssi);
  
}

void updateLedPanel() {
  byte d = garden_payload.getMsgId();
  
  digitalWrite(SR_LATCH, LOW);
  // Bit shift left as there is no 8th led.
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, d << 1);
  digitalWrite(SR_LATCH, HIGH);
}



