/**
 * Listen to the RF transmitter and print it to the serial monitor.
 */
#include <VirtualWire.h>


// Pin 11 is the default receiver pin.
// @see http://www.airspayce.com/mikem/arduino/VirtualWire_8h.html#ae62b601260ae59e7e83c1e63ae0c064b

int received_reset_interval = 60 * 60 * 1000; // How often to reset the received counter.
int received_count; // How many transmissions have been received.
unsigned long received_last_reset;

void setup() 
{
  pinMode(13, OUTPUT);
  
  // initialize serial communication
  Serial.begin(9600);
  Serial.println("setup");
  
  received_count = 0;
  received_last_reset = millis();
  
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
  // vw_set_tx_pin(11);
  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  unsigned long now = millis();
  if (now - received_last_reset > received_reset_interval) {
    received_last_reset = now;  
    received_count = 0;
  }
  
  if (vw_get_message(buf, &buflen)) {
      int i;
      digitalWrite(13, HIGH); // Flash a light to show received good message
      // Message with a good checksum received, output it to serial.
      Serial.print("R: ");
      for (i = 0; i < buflen; i++) {
          Serial.print(char(buf[i]));
      }
      Serial.println("");
      
      ++received_count;
      Serial.print("C: ");
      Serial.println(received_count);
      
      digitalWrite(13, LOW);
  }

}

