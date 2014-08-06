
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <VirtualWire.h>
#include <SoftwareSerial.h>

#define SERIAL_TX_PIN 3
#define SERIAL_RX_PIN 4
#define VW_RX_PIN 2

SoftwareSerial mySerial(SERIAL_RX_PIN, SERIAL_TX_PIN); // RX, TX

void setup()
{

  pinMode(0, OUTPUT);    

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Setup");

  vw_set_rx_pin(VW_RX_PIN);
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver running
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    digitalWrite(0, true);
    
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;
        for (i = 0; i < buflen; i++)
        {
            //Serial.print(char(buf[i]));
            mySerial.write(char(buf[i]));
        }
        mySerial.write('\n'); 
    }

    digitalWrite(0, false);
}
