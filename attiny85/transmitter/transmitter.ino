
#include <util/delay.h>
#include <VirtualWire.h>


#define LED_DEBUG 4
#define PIN_TX 3


byte ledState = LOW;             // ledState used to set the LED
unsigned long msgId = 0;


volatile byte wd_check = 0; // A number for the watchdog timerer to check

ISR(WDT_vect) 
{
  // Check that there have been about 4 seconds, since the last check
  if (wd_check > 2 && wd_check < 6) {
    wd_check = 0;
    // Turn on watchdog again. 
    // See 8.5.2 "To avoid the Watchdog Reset, WDIE must be set after each interrupt"
    WDTCR |= (1 << WDIE);
  }
  // Failed the test so a reset will occur on the next watchdog timeout.
}

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
  watchdog_setup();
  
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec
  vw_set_tx_pin(PIN_TX);
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
  
  ++msgId;
  char msg[16];
  sprintf(msg, "%lu,wd=%d,mv=%u", msgId, wd_check, readVcc());
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  // Cannot use millis() as timer 0 is used by virtualwire on Attiny85
  _delay_ms(1000);
  
  // Detect cpu freeze.
  // Watchdog checks tha about 4 seconds have run since last check.
  // So make sure to record every "second" encountered.
  // If the power is unstable then these seconds will not match with 
  // watchdog's timer, and force a reset.
  ++wd_check;

}

void watchdog_setup()
{
  // WDIE = watchdog interupt enable
  // WDE  = turn on watchdog
  // WDP3 = about 4 seconds
  WDTCR |= (1 << WDIE) | (1 << WDE) | ( 1<< WDP3);
}

