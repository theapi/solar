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

