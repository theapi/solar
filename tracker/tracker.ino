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
// Investigate replacing Servo with ServoTimer2
// So that virtual wire can be used.
// virtualwire & servo both use timer1
// http://forum.arduino.cc/index.php?PHPSESSID=chto8kajoid84icno5e5sm0jk7&topic=21975.0
#include <Servo.h> 

#define PIN_LDR_UP    0 // PC0
#define PIN_LDR_DOWN  1 // PC1
#define PIN_LDR_LEFT  2 // PC2
#define PIN_LDR_RIGHT 3 // PC3
#define THRESHOLD_LDR_VIRT 25
#define THRESHOLD_LDR_HORZ 25

#define SERVO_PIN_VIRT 8 // PB0
#define SERVO_PIN_HORZ 9 // PB1

// When the last move was attempted.
unsigned long move_last = 0; 
// The move delay.
const long move_interval = 100; 

Servo servo_virt;
Servo servo_horz;

void setup()
{
    Serial.begin(9600); 
    
    servo_virt.attach(SERVO_PIN_VIRT);
    servo_horz.attach(SERVO_PIN_HORZ);
    
    servo_virt.write(110);
    servo_horz.write(90);
}

void loop()
{
    unsigned long now = millis();
    if (now - move_last >= move_interval) {
        // Move the panels if needed.
        tkr_move();
    }
}

void tkr_move()
{
    int diff_virt = tkr_diff_virt();
    int diff_horz = tkr_diff_horz();
    Serial.println(); 
    
    if (diff_virt) {
      // move up or down
      int deg = servo_virt.read();
      
      if (diff_virt > 0) {
        ++deg;
        if (deg > 160) deg = 160;
      } else {
        --deg;
        if (deg < 40) deg = 40;
      }
      
      Serial.print(diff_virt);
      Serial.print('-');
      Serial.println(deg); 
      
      // move a degree each loop until target is reached.
      servo_virt.attach(SERVO_PIN_VIRT);
      servo_virt.write(deg);
      
    }

    if (diff_horz) {
      // move left or right.
      int deg = servo_horz.read();
      
      if (diff_horz > 0) {
        ++deg;
        if (deg > 170) deg = 170;
      } else {
        --deg;
        if (deg < 10) deg = 10;
      }
      
      Serial.print(diff_horz);
      Serial.print('-');
      Serial.println(deg); 
      
      // move a degree each loop until target is reached.
      servo_horz.attach(SERVO_PIN_HORZ);
      servo_horz.write(deg);
      
    }
    
}

int tkr_diff_virt()
{
    int up = analogRead(PIN_LDR_UP);
    int down = analogRead(PIN_LDR_DOWN);
    
    Serial.print(up); 
    Serial.print(':');
    Serial.print(down); 
    Serial.print(':');
    
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
    
    Serial.print(left); 
    Serial.print(':');
    Serial.print(right); 
    
    int diff = left - right
    ;
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

