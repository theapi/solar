
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "Payload.h"

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);

Payload rx_payload = Payload();
uint8_t input_string[Payload_SIZE];
uint8_t payload_state = 0;
uint8_t serial_byte_count = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Initialise the display.
  display.init();
 // display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);

}

void loop() {

  display.clear();

  if (payload_state = 2) {
    display.drawString(20, 0, String(rx_payload.getMsgId()));
    display.drawString(20, 40, String(rx_payload.getA()));
    display.drawString(40, 0, String(rx_payload.getB())); 
    payload_state = 0;
  }
  
  display.drawString(0, 0, String(millis()));
  display.display();
  
  delay(10);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    uint8_t in = (uint8_t) Serial.read();
    Serial.print(in);
    if (payload_state == 0) {
      // Check for the start of the payload
      // which is device id "1".
      if (in == 1) {
        payload_state = 1;
      }
    }
    
    if (payload_state == 1) {
      // add it to the inputString:
      input_string[serial_byte_count] = in;
      ++serial_byte_count;
      
      // if the the last byte is received, set a flag
      // so the main loop can do something about it:
      if (serial_byte_count == rx_payload.getPayloadSize()) {
        serial_byte_count = 0;
        payload_state = 2;
        rx_payload.unserialize(input_string);
      }
    }
  }
}

