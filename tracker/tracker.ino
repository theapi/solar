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

#include <util/delay.h>
#include <avr/wdt.h>
//#include <VirtualWire.h>
#include <Servo.h> 
//#include "ServoTimer2.h" 

#define LED_DEBUG 13
#define PIN_TX 7

#define PIN_LDR_UP    0 // PC0
#define PIN_LDR_DOWN  1 // PC1
#define PIN_LDR_LEFT  2 // PC2
#define PIN_LDR_RIGHT 3 // PC3
#define THRESHOLD_LDR_VIRT 40
#define THRESHOLD_LDR_HORZ 40
#define MOVE_INTERVAL_MILLIS 60

#define SERVO_PIN_HORZ 8 // PB1
#define SERVO_PIN_VIRT 9 // PB0

#define SERVO_HORZ_MAX 2500
#define SERVO_HORZ_MIN 700

#define SERVO_VIRT_MAX 1900
#define SERVO_VIRT_MIN 1200


#define THRESHOLD_DARK 30 // A reading below his is considered dark.
#define DARK_SLEEP_COUNT (MOVE_INTERVAL_MILLIS * 10 * 60) // 1 minute of dark

// When the last move was attempted.
unsigned long move_last = 0; 
// The move delay.
const long move_interval = MOVE_INTERVAL_MILLIS; 

// When the last transmit was attempted.
unsigned long tx_last = 0; 
// The transmit interval.
const long tx_interval = 2000; 

byte ledState = HIGH;
byte msgId = 0;

// How many readingas are considered dark.
// If dark for a while, deep sleep can be enabled.
unsigned long tkr_dark_count = 0;


Servo servo_virt;
Servo servo_horz;

// Store the current servo positions. 
// Not using sero.read() as that is degrees.
int servo_virt_pos = 1500;
int servo_horz_pos = 1700;

void setup()
{
    Serial.begin(9600); 
    
    pinMode(LED_DEBUG, OUTPUT);     
    pinMode(PIN_TX, OUTPUT); 
   
    //vw_set_ptt_inverted(true); // Required for DR3100
    //vw_setup(2000);      // Bits per sec
    //vw_set_tx_pin(PIN_TX);
      
    watchdog_setup();
  
  
    servo_virt.attach(SERVO_PIN_VIRT);
    servo_horz.attach(SERVO_PIN_HORZ);
    
    servo_virt.writeMicroseconds(servo_virt_pos); // 1200 -> 1500 -> 1900
    servo_horz.writeMicroseconds(servo_horz_pos); // 700  -> 1700 -> 2500
    
}

void loop()
{
    unsigned long now = millis();
    if (now - move_last >= move_interval) {
        move_last = now;
        
        // Move the panels if needed.
        tkr_move();
        
        
        // Reset watchdog so he knows all is well.
        wdt_reset();
    }
    
    if (now - tx_last >= tx_interval) {
        tx_last = now;
        
        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW) {
          ledState = HIGH;
        } else {
          ledState = LOW;
        }
        //digitalWrite(LED_DEBUG, ledState);
        
        // Send a transmission
        //char msg[16];
        //sprintf(msg, "%d,wd=%lu,mv=%u", msgId, tkr_dark_count, readVcc());
        //vw_send((uint8_t *)msg, strlen(msg));
        //vw_wait_tx(); // Wait until the whole message is gone
        
        
        if (tkr_dark_count > DARK_SLEEP_COUNT) {
          tkr_dark_count = 0;  
          //@todo goto sleep(), it's too dark.
        }
        
 
    }
}

void tkr_move()
{
    int diff_virt = tkr_diff_virt();
    int diff_horz = tkr_diff_horz();
    //Serial.println(); 
    
    if (diff_virt) {
      // move up or down

      if (diff_virt > 0) {
        servo_virt_pos += 10;
        if (servo_virt_pos > SERVO_VIRT_MAX) servo_virt_pos = SERVO_VIRT_MAX;
      } else {
        servo_virt_pos -= 10;
        if (servo_virt_pos < SERVO_VIRT_MIN) servo_virt_pos = SERVO_VIRT_MIN;
      }
      
      Serial.print("  > ");
      Serial.print(diff_virt);
      Serial.print('-');
      Serial.println(servo_virt_pos); 
      
      // move a degree each loop until target is reached.
      //servo_virt.attach(SERVO_PIN_VIRT);
      servo_virt.writeMicroseconds(servo_virt_pos);
      
    }

    if (diff_horz) {
      // move left or right.
      
      if (diff_horz > 0) {
        servo_horz_pos += 10;
        if (servo_horz_pos > SERVO_HORZ_MAX) servo_horz_pos = SERVO_HORZ_MAX;
      } else {
        servo_horz_pos -= 10;
        if (servo_horz_pos < SERVO_HORZ_MIN) servo_horz_pos = SERVO_HORZ_MIN;
      }
      
      Serial.print("  > ");
      Serial.print(diff_horz);
      Serial.print("-");
      Serial.println(servo_horz_pos); 
      
      // move a degree each loop until target is reached.
      //servo_horz.attach(SERVO_PIN_HORZ);
      servo_horz.writeMicroseconds(servo_horz_pos);
      
    }
    
}

int tkr_diff_virt()
{
    int up = analogRead(PIN_LDR_UP);
    int down = analogRead(PIN_LDR_DOWN);
    
    // Count the dark readings.
    if (up < THRESHOLD_DARK) {
      //++tkr_dark_count;  
    }
    
    /*
    Serial.print(up); 
    Serial.print(":");
    Serial.print(down); 
    Serial.print(":");
    */
    
    int diff = up - down;
    if (abs(diff) > THRESHOLD_LDR_VIRT) {
      return diff;
    }
    
    return 0;
}

int tkr_diff_horz()
{
    int left = analogRead(PIN_LDR_LEFT);
    int right = analogRead(PIN_LDR_RIGHT);
    
    /*
    Serial.print(left); 
    Serial.print(':');
    Serial.print(right); 
    */
    
    int diff = left - right;
    if (abs(diff) > THRESHOLD_LDR_HORZ) {
      return diff;
    }
    
    return 0;
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

void watchdog_setup()
{
  // Reset after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  wdt_enable(WDTO_8S);
}



