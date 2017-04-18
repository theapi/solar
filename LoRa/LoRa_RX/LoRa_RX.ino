/**
 * Listen for LoRa transmissions,
 * then pass the data through Serial to be processed by the monitor.
 */

#include <SPI.h>
#include <RH_RF95.h>
// oled display
#include "U8glib.h"
#include "Payload.h"


#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

// UK frequency
#define RF95_FREQ 868.0

// Blinky on receipt
#define LED 13

// Shift register pins for the LEDs on the front panel.
#define SR_CLK 5
#define SR_DATA 6
  
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// OLED
U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);

Payload tx_payload = Payload();

typedef struct{
  int num;
  int rssi;
}
monitor_t;
monitor_t monitor;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SR_CLK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);

  //u8g.setRot180();
  monitor.num = 0;
  monitor.rssi = 0;
  displayUpdate();
  
  delay(100);

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
      int num = atoi((char*)buf);
      Serial.println(num);
      Serial.print("RSSI: ");

      int rssi = rf95.lastRssi();
      Serial.println(rf95.lastRssi(), DEC);

      tx_payload.setDeviceId(1);
      tx_payload.setMsgId(num % 255);
      tx_payload.setA(num);
      tx_payload.setB(rssi);
      tx_payload.setC(72);
      tx_payload.setD(69);
      tx_payload.setE(76);
      tx_payload.setF(79);

      uint8_t payload_buf[Payload_SIZE];
      tx_payload.serialize(payload_buf);
      Serial.write('\t'); // Payload start byte
      Serial.write(payload_buf, Payload_SIZE);

      monitor.num = num;
      monitor.rssi = rssi;
      
      // Send a reply
      char radiopacket[30]   = "                   ";
      itoa(num, radiopacket, 10);
      rf95.send(radiopacket, sizeof(radiopacket));
      rf95.waitPacketSent();
      //Serial.println("Sent a reply");

      displayUpdate();
      updateFrontPanel();
      
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

void updateFrontPanel()
{
   // NB the 74HC595 has latch tied to clock 
   // so latch happens every clock
   // latching data from the previous clock.
   // So send an extra bit for the final latch.
   byte leds = 0B11111111; // tmp
   shiftOut(SR_DATA, SR_CLK, LSBFIRST, leds);
}


