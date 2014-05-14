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
#include <Wire.h>
#include <Servo.h> 
 

#define LED_DEBUG 13
#define PIN_TX 7

#define PIN_LDR_LEFT  2 // PC2
#define PIN_LDR_RIGHT 3 // PC3
#define THRESHOLD_LDR_HORZ 50
#define MOVE_INTERVAL_MILLIS 60

#define SERVO_INCREMENT 5 // How much to move each loop
#define SERVO_PIN_HORZ 8 // PB1
#define SERVO_HORZ_MAX 2800
#define SERVO_HORZ_MIN 700


#define I2C_SLAVE_ADDR 100

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


Servo servo_horz;

// Store the current servo positions. 
int servo_horz_pos = 1700;

void setup()
{
    watchdog_setup();
    
    Serial.begin(9600); 
    
    pinMode(LED_DEBUG, OUTPUT);     
    pinMode(PIN_TX, OUTPUT); 

    servo_horz.attach(SERVO_PIN_HORZ, SERVO_HORZ_MIN, SERVO_HORZ_MAX);

    servo_horz.writeMicroseconds(servo_horz_pos);
    
    // Join the i2c bus as master
    Wire.begin();
    
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
        
    }
}

void tkr_move()
{
    int diff_horz = tkr_diff_horz();
    //Serial.println(); 

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


void watchdog_setup()
{
  
  // Clear any previous watchdog interupt
  MCUSR = 0;
  
  // Reset after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  wdt_enable(WDTO_8S);
}



