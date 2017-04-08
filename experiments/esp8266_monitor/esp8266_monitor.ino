
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);

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
  display.drawString(0, 0, String(millis()));
  display.display();
  
  delay(10);
}
