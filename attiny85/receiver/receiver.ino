
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <VirtualWire.h>
#include <SoftwareSerial.h>

#define SERIAL_TX_PIN 3
#define SERIAL_RX_PIN 4

#define VW_MAX_MESSAGE_LEN 40 // Same as solar/sensor
#define VW_RX_PIN 2

SoftwareSerial mySerial(SERIAL_RX_PIN, SERIAL_TX_PIN); // RX, TX

const byte debug_led = 0;

byte pwm_val = 30;


void setup()
{

  pinMode(debug_led, OUTPUT);    
  
  pinMode(1, OUTPUT); // PWM on timer1
  initPWM();

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
  
  digitalWrite(debug_led, HIGH);
  
  if (vw_get_message(buf, &buflen)) { // Non-blocking
    int i;
    int val = 0;
    byte comma = 0;
    for (i = 0; i < buflen; i++) {
      
      
      
      if (buf[i] == 44) {      
        // CSV - S,88,20.17,21.91,871,5595,5436
        // comma 5 = solar panel reading
        // comma 6 = battery reading
        
        if (comma == 5) {
          pwm_val = map(val, 0, 10000, 0, 255);
          setPwm(pwm_val);
          
          // Print the solar reading
          //mySerial.print(" pwm("); 
          //mySerial.print(pwm_val, DEC); 
          //mySerial.print(")"); 
        }
        
        ++comma;
        val = 0;
      }
      
      
      mySerial.write(char(buf[i]));
      
      
      if (comma == 5 || comma == 6) {
        byte c = buf[i];
        // Only interested in numbers.
        // ASCII hex values 0x30 to 0x30 are decimal 0 to 9
        if (c > 0x2F && c < 0x3A) {
            if (val > 0) {
                // Expecting decimal numbers like 24,
                // so for each new argument multiply by ten.
                val *= 10;
                val += c - 0x30;
            } else {
                val = c - 0x30;
            }
            //mySerial.print(val, DEC); 
        }
      }
      
      
    }
    
    
    // Print the battery reading
    //mySerial.print(" Bat("); 
    //mySerial.print(val, DEC); 
    //mySerial.print(")");
    
    
    mySerial.println(); 
    
  }

  digitalWrite(debug_led, LOW);
}

void setPwm(byte val)
{
  // Set the compare value
  OCR1A = val;
}

void initPWM()
{
  // Fast PWM on timer 2 
  // PB1 ONLY (non inverting)
  TCCR1 = (1 << PWM1A) | (1 << COM1A1) | (0 << COM1A0) | (1 << CTC1) | (1 << CS10);
  setPwm(pwm_val);
}
