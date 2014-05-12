/**
 *
 
Attiny85v
- If not enough light SLEEP.
- Turns on by high on pin interrupt. 
  The pin is attached to the voltage divider that measures the resistance of the “up” LDR. 
  When dark, the voltage is less than the voltage required to register as HIGH.
- Measures the battery voltage. 
- Every minute (may need to reduce this) sends data over RF 
- If battery voltage is high enough, the atmega328p is woken by pin interrupt going HIGH.

 *
 */
 
// ATMEL ATTINY 25/45/85 / ARDUINO
//
//                  +-\/-+
// Ain0 (D 5) PB5  1|    |8  Vcc
// Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1
// Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
//            GND  4|    |5  PB0 (D 0) pwm0
//                  +----+
// see http://www.gammon.com.au/forum/?id=11488&reply=9#reply9

 
#include <util/delay.h>
#include <avr/wdt.h>      // Watchdog
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <VirtualWire.h>


#define LED_DEBUG 0 
#define PIN_TX 3 // PB3

#define PIN_WAKEUP_MASTER 2 // PB2 (SCK) Connected to the atmega328 external interupt pin & SCK.

#define PIN_WAKEUP_SELF 4   // PB4 / pin 3 / same as PIN_LDR
#define PIN_LDR 2           // PB4 / pin 3 / A2 - NB: ANALOG pin 2 not digital pin 2

#define THRESHOLD_DARK 500 // A reading below this is considered dark.

byte msgId = 0;
int ldr_val = 0;

ISR (PCINT0_vect)  
{
  // Wake up
  
}

void setup() 
{
  pinMode(LED_DEBUG, OUTPUT);     
  pinMode(PIN_TX, OUTPUT); 
  pinMode(PIN_WAKEUP_SELF, INPUT);
  pinMode(PIN_WAKEUP_MASTER, OUTPUT); 
  
  digitalWrite(PIN_WAKEUP_MASTER, LOW);
  
  
  // pin change interrupt (example for D4)
  PCMSK  |= bit (PCINT4);  // want pin D4 / pin 3
  GIFR   |= bit (PCIF);    // clear any outstanding interrupts
  GIMSK  |= bit (PCIE);    // enable pin change interrupts 
  
 
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
  vw_set_tx_pin(PIN_TX);
    
  //watchdog_setup();
  
}

void loop()
{
  
  int ldr_val = analogRead(PIN_LDR);

  digitalWrite(LED_DEBUG, HIGH);
  
  
  long vcc = readVcc();

  char msg[16];
  sprintf(msg, "%d,%d,%u", msgId, ldr_val, vcc);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(LED_DEBUG, LOW);
  
  // Reset watchdog so he knows all is well.
  //wdt_reset();

  // @todo Only signal master wake condition if not in SPI mode.
  if (ldr_val < THRESHOLD_DARK) {
    // Too dark
    // Let atmega328p know it should be asleep
    digitalWrite(PIN_WAKEUP_MASTER, LOW);
    
    // Sleep untill enough light
    goToSleep();
  } else if (vcc >= 3300) {
    // Tell the Atmega328p that it should be awake now.
    digitalWrite(PIN_WAKEUP_MASTER, HIGH);
  }
  
  
  // Cannot use millis() as timer 0 is used by virtualwire on Attiny85
  _delay_ms(1000);

  ++msgId;
  

}


void watchdog_setup()
{
  // Reset after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  wdt_enable(WDTO_8S);
}

void goToSleep()
{
  digitalWrite(PIN_WAKEUP_MASTER, LOW); // sleep master
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0;            // turn off ADC
  power_all_disable ();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu();                             
  sleep_disable();   
  power_all_enable();    // power everything back on
  
  ADCSRA = (1 << ADEN); // ADC back on
} 


/**
 * Read the interanl voltage.
 */
long readVcc() 
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  _delay_ms(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

