/*
ATTINY85

ATMEL ATTINY 25/45/85 / ARDUINO
                              +-\/-+
     RESET - Ain0 (D 5) PB5  1|    |8  Vcc
 Serial TX - Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1 - VirtualWire RX
 Serial RX - Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1 - Volmeter
                        GND  4|    |5  PB0 (D 0) pwm0 - RF Receiver power
                              +----+
 see http://www.gammon.com.au/forum/?id=11488&reply=9#reply9
*/

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
#define RF_POWER_PIN 0 // Toggle power to RF receiver

#define WD_DO_STUFF 1 // How many watchdog interupts before doing real work.


SoftwareSerial mySerial(SERIAL_RX_PIN, SERIAL_TX_PIN); // RX, TX


byte pwm_val = 127; // Temporary half value
volatile byte wd_isr = WD_DO_STUFF;

byte count = 10;

ISR(WDT_vect)
{
  // Wake up by watchdog
  if (wd_isr == 0) {
      wd_isr = WD_DO_STUFF;
  } else {
      --wd_isr; 
      // Go back to sleep.
      goToSleep();
  }
}

void setup()
{
  // disable ADC
  ADCSRA = 0;
  
  // disable analog comparitor
  ACSR = (1 << ACD) | (0 << ACIE);
  
  watchdog_setup();
  pinMode(RF_POWER_PIN, OUTPUT);  
  digitalWrite(RF_POWER_PIN, HIGH);  
  
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
  --count;
  if (count == 0) {
    count = 10;
    goToSleep(); 
  }
  
  // Reset watchdog so he knows all is well.
  wdt_reset();
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  
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

  
  _delay_ms(500); // VERY TEMP
}

void setPwm(byte val)
{
  // Set the compare value
  OCR1A = val;
}

void initPWM()
{
  // Fast PWM on timer 1 
  // PB1 ONLY (non inverting)
  TCCR1 = (1 << PWM1A) | (1 << COM1A1) | (0 << COM1A0) | (1 << CTC1) | (1 << CS10);
  setPwm(pwm_val);
}

/**
 * Watchdog for while awake to ensure things are ticking over.
 */
void watchdog_setup()
{
  // Clear any previous watchdog interupt
  MCUSR = 0;

  // WD interrupt after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCR = 1<<WDP0 | 1<<WDP3; // 8.0 seconds 
  
  /* Enable the WD interrupt (note no reset). */
  WDTCR |= _BV(WDIE);
  
  //wdt_enable(WDTO_8S);
  wdt_reset();
}

void goToSleep()
{

  // Turn off the RF receiver
  digitalWrite(RF_POWER_PIN, LOW);
  
  
  cli();
  
  // ensure ADC is off
  ADCSRA = 0;
  
  set_sleep_mode(SLEEP_MODE_IDLE);

  
  // Power-down unused stuff - keep timer1 running for the pwm
  // off - timer0, USI, ADC
  PRR = (0 << PRTIM1) | (1 << PRTIM0) | (1 << PRUSI) | (1 << PRADC); 
  
  sleep_enable();
  sei();
  
  // turn off brown-out enable in software
  //MCUCR = (1 << BODS) | (1 << BODSE); // turn on brown-out enable select
  //MCUCR = (1 << BODS) | (0 << BODSE); // this must be done within 4 clock cycles of above
  //MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
  //MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
  sleep_cpu();              // sleep within 3 clock cycles of above

                              
  sleep_disable();  
  MCUSR = 0; // clear the reset register 
  
  // Turn timer0 back on
  PRR = (0 << PRTIM1) | (0 << PRTIM0) | (1 << PRUSI) | (1 << PRADC);

  // turn on the RF receiver
  digitalWrite(RF_POWER_PIN, HIGH);
  
} 
