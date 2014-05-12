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


#define LED_DEBUG 4 // NB: this is the same pin as the master wake up, so for debugging only!
#define PIN_TX 3
#define PIN_WAKEUP_SELF 2   // pin 2 / PCINT2 / INT0 / external interupt pin.
#define PIN_WAKEUP_MASTER 4 // Connected to the atmega328 external interupt pin.

#define THRESHOLD_DARK 50 // A reading below his is considered dark.

byte msgId = 0;
int ldr_val = 0;

ISR (INT0_vect) 
{
  // Wake up
  
}

void setup() 
{
  pinMode(LED_DEBUG, OUTPUT);     
  pinMode(PIN_TX, OUTPUT); 
  pinMode(PIN_WAKEUP_SELF, INPUT);
  pinMode(PIN_WAKEUP_MASTER, OUTPUT); 
  
  digitalWrite(PIN_WAKEUP_SELF, HIGH);  // internal pull-up
  
  // External interrupt
  MCUCR |= bit(ISC01) | bit(ISC00); // The rising edge of INT0 generates an interrupt request.
  GIFR  |= bit(INTF0);   // clear any outstanding interrupts
  GIMSK |= bit(INTO);    // enable external interrupts 
 
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
  vw_set_tx_pin(PIN_TX);
    
  watchdog_setup();
}

void loop()
{
  
  ldr_val = analogRead(PIN_WAKEUP_SELF);

  digitalWrite(LED_DEBUG, HIGH);
  
  long vcc = readVcc();

  char msg[16];
  sprintf(msg, "%d,wd=%d,mv=%u", msgId, ldr_val, vcc);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(LED_DEBUG, LOW);
  
  if (vcc >= 3300) {
    // Tell the Atmega328p that it should be awake now.
    digitalWrite(PIN_WAKEUP_MASTER, HIGH);
  }
  
  
  // Cannot use millis() as timer 0 is used by virtualwire on Attiny85
  _delay_ms(1000);
  
  // Reset watchdog so he knows all is well.
  wdt_reset();

  ++msgId;
  
  if (ldr_val < THRESHOLD_DARK) {
    // Too dark
    // Let atmega328p know it should be asleep
    digitalWrite(PIN_WAKEUP_MASTER, LOW);
    
    // Sleep untill enough light
    goToSleep();
  }
}

void watchdog_setup()
{
  // Reset after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  wdt_enable(WDTO_8S);
}

void goToSleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0;            // turn off ADC
  power_all_disable ();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu();                             
  sleep_disable();   
  power_all_enable();    // power everything back on
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

