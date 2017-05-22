// Arduino9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Arduino9x_TX

#include <SPI.h>
#include <RH_RF95.h>
// oled display
//#include "U8glib.h"


#define RFM95_CS D1
//#define RFM95_RST 2
#define RFM95_INT D2

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0



// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// OLED
//U8GLIB_SSD1306_128X64_2X u8g(U8G_I2C_OPT_NONE);

typedef struct{
  int num;
  int rssi;
}
monitor_t;
monitor_t monitor;

void setup() 
{
  //pinMode(LED, OUTPUT);     
  //pinMode(RFM95_RST, OUTPUT);
  //digitalWrite(RFM95_RST, HIGH);


SPI.setFrequency(8000000);
  Serial.begin(115200);

  //u8g.setRot180();
  monitor.num = 0;
  monitor.rssi = 0;
  displayUpdate();
  
  delay(100);

  Serial.println("Arduino LoRa RX Test!");
  
  // manual reset
//  digitalWrite(RFM95_RST, LOW);
//  delay(10);
//  digitalWrite(RFM95_RST, HIGH);
//  delay(10);

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

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      //digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
      int num = atoi((char*)buf);
      Serial.println(num);
       Serial.print("RSSI: ");

      int rssi = rf95.lastRssi();
      Serial.println(rf95.lastRssi(), DEC);

      monitor.num = num;
      monitor.rssi = rssi;
      displayUpdate();
      
      // Send a reply
      //uint8_t data[] = "And hello back to you";
      uint8_t radiopacket[30]   = "                   ";
      itoa(num, (char*)radiopacket, 10);
      rf95.send(radiopacket, sizeof(radiopacket));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      //digitalWrite(LED, LOW);




      
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}

void displayUpdate()
{
//  // picture loop
//  u8g.firstPage();
//  do {
//    draw();
//  } while( u8g.nextPage() );
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
//
//  u8g.setFont(u8g_font_fub11n);
//  //u8g.setFont(u8g_font_unifont);
//  u8g.setFontPosTop();
//  
//  u8g.setPrintPos(0, 0);
//  u8g.print(monitor.num);
//  
//  u8g.setPrintPos(0, 20);
//  u8g.print(monitor.rssi);
  
}

