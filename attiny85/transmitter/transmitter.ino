
#include <util/delay.h>
#include <avr/wdt.h>
#include <VirtualWire.h>


#define LED_DEBUG 4
#define PIN_TX 3


byte ledState = HIGH;             // ledState used to set the LED
byte msgId = 0;
byte wd_check = 0;

long readVcc() {
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

void setup() 
{
  pinMode(LED_DEBUG, OUTPUT);     
  pinMode(PIN_TX, OUTPUT); 
 
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
  vw_set_tx_pin(PIN_TX);
    
  watchdog_setup();
}

void loop()
{
  
  // if the LED is off turn it on and vice-versa:
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(LED_DEBUG, ledState);
  
  
  char msg[16];
  sprintf(msg, "%d,wd=%d,mv=%u", msgId, wd_check, readVcc());
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  // Cannot use millis() as timer 0 is used by virtualwire on Attiny85
  _delay_ms(1000);
  
  // Reset watchdog so he knows all is well.
  wdt_reset();

  wd_check = 1;
  ++msgId;
}

void watchdog_setup()
{
  // Reset after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  wdt_enable(WDTO_8S);
}

