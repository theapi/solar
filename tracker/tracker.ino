/* 

Connect the photoresistor one leg to pin 0, and pin to +5V
Connect a resistor from pin 0 to GND.
Need to try in sunlight to get better idea of good resistor value.

----------------------------------------------------

           PhotoR     10K
 +5    o---/\/\/--.--/\/\/---o GND
                  |
 Pin 0 o-----------

----------------------------------------------------
*/


/*
Hacked virtualwire to use 8 bit timer2
from line 510...

#else // ARDUINO - hacked to timer2 http://forum.arduino.cc/index.php/topic,100371.0.html
    // figure out prescaler value and counter match value
    prescaler = vw_timer_calc(speed, (uint8_t)-1, &nticks);
    if (!prescaler)
    {
        return; // fault
    }

    TCCR2A = 0;
    TCCR2A = _BV(WGM21); // Turn on CTC mode / Output Compare pins disconnected

    // convert prescaler index to TCCRnB prescaler bits CS00, CS01, CS02
    TCCR2B = 0;
    TCCR2B = prescaler; // set CS00, CS01, CS02 (other bits not needed)

    // Number of ticks to count before firing interrupt
    OCR2A = uint8_t(nticks);

    // Set mask to fire interrupt when OCF0A bit is set in TIFR0
    TIMSK2 |= _BV(OCIE2A);
    // Enable interrupt
#ifdef TIMSK1
    // atmega168
    //TIMSK1 |= _BV(OCIE1A);
#else
    // others
    //TIMSK |= _BV(OCIE1A);
#endif // TIMSK1



from line 777...

	#else // Assume Arduino Uno (328p or similar) hack to timer2
		#define VW_TIMER_VECTOR TIMER2_COMPA_vect


*/

#define VW_TIMER_VECTOR TIMER2_COMPA_vect

#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <VirtualWire.h> // hacked version for timer2!!
#include <Servo.h> 
 

#define LED_DEBUG 13
#define PIN_TX 6          // PD6
#define PIN_PERIF_POWER 7 // PD7

#define PIN_LDR_LEFT  0 // PC0
#define PIN_LDR_RIGHT 1 // PC1
#define THRESHOLD_LDR_HORZ 50
#define MOVE_INTERVAL_MILLIS 60

#define SERVO_INCREMENT 5 // How much to move each loop
#define SERVO_PIN_HORZ 9 // PB1
#define SERVO_HORZ_MAX 2800
#define SERVO_HORZ_MIN 700
#define SERVO_PIN_HORZ_POWER 8 // PB0

#define DARK_THRESHOLD 500 // too dark below this

#define SLEEP_FLAG_DARK 0  // Gone to sleep because it is dark
#define SLEEP_FLAG_IDLE 1  // Gone to sleep because nothing to do

// When the last move was attempted.
unsigned long move_last = 0; 
// The move delay.
const long move_interval = MOVE_INTERVAL_MILLIS; 

// When the last transmit was attempted.
unsigned long tx_last = 0; 
// The transmit interval.
const long tx_interval = 2000; 

int light_level = 0; // latest light reading from the LDR

byte ledState = HIGH;
byte msgId = 0;

byte sleep_flag;

Servo servo_horz;

// Store the current servo positions. 
int servo_horz_pos = 1700;

/**
 * ISR for INT0
 */
void wakeUp() 
{
  // Just wake up
}

void setup()
{
    watchdog_setup();
    
    Serial.begin(9600); 
    
    pinMode(LED_DEBUG, OUTPUT);     
    pinMode(PIN_TX, OUTPUT); 
    
    pinMode(PIN_PERIF_POWER, OUTPUT);
    digitalWrite(PIN_PERIF_POWER, LOW); // low = 0n (PNP)
    
    pinMode(SERVO_PIN_HORZ_POWER, OUTPUT); 
    digitalWrite(SERVO_PIN_HORZ_POWER, LOW); // low = 0n (PNP)
    
    
    //vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec
    vw_set_tx_pin(PIN_TX);

    servo_horz.attach(SERVO_PIN_HORZ, SERVO_HORZ_MIN, SERVO_HORZ_MAX);
    servo_horz.writeMicroseconds(servo_horz_pos);

}

void loop()
{
    unsigned long now = millis();
    if (now - move_last >= move_interval) {
        move_last = now;
        
        // Reset watchdog so he knows all is well.
        wdt_reset();
        
        // Move the panels if needed.
        tkr_move();
        
        if (light_level < DARK_THRESHOLD) {
          //goToSleep(SLEEP_FLAG_DARK);
          //TODO connect PIN_LDR_LEFT to INT0
        }
        
    }
    
    // Transmit after interval
    if (now - tx_last >= tx_interval) {
        tx_last = now;
        
        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW) {
          ledState = HIGH;
        } else {
          ledState = LOW;
        }
        digitalWrite(LED_DEBUG, ledState);
        
        // Send a transmission
        char msg[16];
        sprintf(msg, "%d,wd=%u,mv=%lu", msgId, light_level, readVcc());
        vw_send((uint8_t *)msg, strlen(msg));
        vw_wait_tx(); // Wait until the whole message is gone
        ++msgId;
        
        // Watchdog will wake us up in 8 seconds time.
        //goToSleep(SLEEP_FLAG_IDLE);
    }
}

void tkr_move()
{
    int diff_horz = tkr_diff_horz();

    if (diff_horz) {
      // move left or right.
      
      if (diff_horz > 0) {
        servo_horz_pos += SERVO_INCREMENT;
        if (servo_horz_pos > SERVO_HORZ_MAX) servo_horz_pos = SERVO_HORZ_MAX;
      } else {
        servo_horz_pos -= SERVO_INCREMENT;
        if (servo_horz_pos < SERVO_HORZ_MIN) servo_horz_pos = SERVO_HORZ_MIN;
      }
      
      Serial.print("  H > ");
      Serial.print(diff_horz);
      Serial.print("-");
      Serial.println(servo_horz_pos); 
      
      // move a bit each loop until target is reached.
      servo_horz.writeMicroseconds(servo_horz_pos);
      
    }
    
}

int tkr_diff_horz()
{
    int left = analogRead(PIN_LDR_LEFT);
    int right = analogRead(PIN_LDR_RIGHT);
    
    light_level = left;
    
    Serial.print(left); 
    Serial.print(':');
    Serial.println(right); 
    
    
    int diff = left - right;
    if (abs(diff) > THRESHOLD_LDR_HORZ) {
      return diff;
    }
    
    return 0;
}

/**
 * Watchdog for while awake to ensure things are ticking over.
 */
void watchdog_setup()
{
  // Clear any previous watchdog interupt
  MCUSR = 0;
  
  // Reset after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  wdt_enable(WDTO_8S);
  wdt_reset();
}

void goToSleep(byte flag)
{
  sleep_flag = flag;
  
  // Power down the periferals
  digitalWrite(PIN_PERIF_POWER, HIGH);
  
  cli();
  
  
  // disable ADC
  ADCSRA = 0;  
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  if (sleep_flag == SLEEP_FLAG_DARK) {
    // Turn off watchdog
    wdt_disable();
    // Attach the external interupt to the LDR.
    attachInterrupt(0, wakeUp, HIGH);
  }

  power_all_disable();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sei();
  
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
  MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
  sleep_cpu();              // sleep within 3 clock cycles of above

                              
  sleep_disable();  
  MCUSR = 0; // clear the reset register 
  

  if (sleep_flag == SLEEP_FLAG_DARK) {
    detachInterrupt(0); // No wakeup interupt while awake
    watchdog_setup(); // watchdog back on
  }
  
  power_all_enable();    // power everything back on
  
  ADCSRA = (1 << ADEN); // ADC back on
  
  // Power up the periferals
  digitalWrite(PIN_PERIF_POWER, LOW); // (PNP)
} 

/**
 * Read the internal voltage.
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

