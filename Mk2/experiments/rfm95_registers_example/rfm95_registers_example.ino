

#include <SPI.h>


#include <RH_RF95.h>


#define RFM95_CS 10
//#define RFM95_RST 2
#define RFM95_INT 3

#define RF95_FREQ 868.0


// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);



void setup() {
  Serial.begin(115200);

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

}

void loop() {

  rf95.setModeIdle();
  delay(100);

  rf95.printRegisters();

  //rf95.sleep();
  delay(1000);  

}

