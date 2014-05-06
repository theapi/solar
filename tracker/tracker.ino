/* 

Connect the photoresistor one leg to pin 0, and pin to +5V
Connect a resistor (experimenting 100ohm to 1k seem good) from pin 0 to GND.
Need to try in sunlight to get better idea of good resistor value.

----------------------------------------------------

           PhotoR     1K
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
#define THRESHOLD_LDR_VIRT 20
#define THRESHOLD_LDR_HORZ 20

#define SERVO_PIN_VIRT 8 // PB0
#define SERVO_PIN_HORZ 9 // PB1

Servo servo_virt;
Servo servo_horz;

void setup()
{
    Serial.begin(9600); 
    
    servo_virt.attach(SERVO_PIN_VIRT);
    servo_horz.attach(SERVO_PIN_HORZ);
}

void loop()
{
    // Move the panels if needed.
    tkr_move();
    delay(100); 
}

void tkr_move()
{
    int dist_virt = tkr_dist_virt();
    int dist_horz = tkr_dist_horz();
    Serial.println(); 
    
    if (dist_virt) {
      // move up or down, within range.
      byte deg = map(dist_virt, 100, 1023, 0, 170);
      servo_virt.write(deg);
    }

    if (dist_horz) {
      // move left or right, within range.
      byte deg = map(dist_horz, 0, 1023, 0, 180);
      servo_horz.write(deg);
    }
    
}

int tkr_dist_virt()
{
    int up = analogRead(PIN_LDR_UP);
    int down = analogRead(PIN_LDR_DOWN);
    
    Serial.print(up); 
    Serial.print(':');
    Serial.print(down); 
    Serial.print(':');
    
    int diff = abs(up - down);
    if (diff > THRESHOLD_LDR_VIRT) {
      return diff;
    }
    
    return 0;
}

int tkr_dist_horz()
{
    int left = analogRead(PIN_LDR_LEFT);
    int right = analogRead(PIN_LDR_RIGHT);
    
    Serial.print(left); 
    Serial.print(':');
    Serial.print(right); 
    Serial.print(':');
    
    int diff = abs(left - right);
    if (diff > THRESHOLD_LDR_HORZ) {
      return diff;
    }
    
    return 0;
}

