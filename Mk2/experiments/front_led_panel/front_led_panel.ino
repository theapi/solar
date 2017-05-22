


// Shift register pins for the LEDs on the front panel.
#define SR_LATCH 16
#define SR_CLK 5
#define SR_DATA 4
  

byte data[8];
byte count = 0;


void setup() {
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_CLK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);

// The panel has only 7 leds, so the LSB is lost.
  data[0] = 0b11111110;
  data[1] = 0b01111110;
  data[2] = 0b00111110;
  data[3] = 0b00011110; 
  data[4] = 0b00001110; 
  data[5] = 0b00000110; 
  data[6] = 0b00000010;
  data[7] = 0b00000000;

}

void loop() {
   
 for (int i = 0; i < 8; i++) {
  updateLedPanel(data[i]);
  delay(500);
 }

 for (int i = 7; i >= 0; i--) {
  updateLedPanel(data[i]);
  delay(500);
 }

 // binary count
 for (int i = 0; i < 127; i++) {
  // Bit shift left as there is no 8th led.
  updateLedPanel(i << 1);
  delay(500);
 }
   
}

void updateLedPanel(byte d) {
  digitalWrite(SR_LATCH, LOW);
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, d);
  digitalWrite(SR_LATCH, HIGH);
}


