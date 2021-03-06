
/**
 * Listen to the RF transmitter and print it to the serial monitor.
 */
 
#include <avr/wdt.h>
#include <VirtualWire.h>
#include <math.h>

#define DEBUG_PIN 13
#define BREATH_PIN 3

// Pin 11 is the default receiver pin.
// @see http://www.airspayce.com/mikem/arduino/VirtualWire_8h.html#ae62b601260ae59e7e83c1e63ae0c064b
#define VW_RX_PIN 7
#define VW_MAX_MESSAGE_LEN 40 // Same as solar/sensor

int heartbeat_interval = 32 * 1000; // How often to send the heartbeat.
unsigned long heartbeat_last;

void setup() 
{
  // Clear any previous watchdog interupt
  MCUSR = 0;
  wdt_enable(WDTO_8S);
  
  pinMode(DEBUG_PIN, OUTPUT);
  

  vw_set_rx_pin(VW_RX_PIN);
  vw_setup(2000);      // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  

  // initialize serial communication
  Serial.begin(9600);
  Serial.println("setup");
}

void loop()
{
  
  // Pat the dog
  wdt_reset();
  
  breath();
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  unsigned long now = millis();
  if (now - heartbeat_last > heartbeat_interval) {
    heartbeat_last = now;
    Serial.print("HeartBeat: ");
    Serial.println(now);
  }
  
  if (vw_get_message(buf, &buflen)) {
      int i;
      digitalWrite(DEBUG_PIN, HIGH); // Flash a light to show received good message
      // Message with a good checksum received, output it to serial.
      Serial.print("R: ");
      for (i = 0; i < buflen; i++) {
          Serial.print(char(buf[i]));
      }
      Serial.println("");
      
      digitalWrite(DEBUG_PIN, LOW);
  }

}

/**
 * Indicate gently that everything is ok.
 */
void breath()
{
  // http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
  
  float val = (exp(sin(millis()/4000.0*PI)) - 0.36787944)*108.0;
  analogWrite(BREATH_PIN, constrain(val, 5, 200));
}

