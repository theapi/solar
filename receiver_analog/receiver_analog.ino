/**
 Receive the sensor's transmission, then display it on the analogue meter.
 */

#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <VirtualWire.h>

#define DEBUG_LED_PIN 5 // PD5 (11) - Red led button
#define DEBUG_LED_PWM_AWAKE 127 // pwm value to show contoller is awake
#define DEBUG_LED_PWM_MSG 255 // pwm value to show message received

#define PWM_PIN 9 // TMP until dedicated chip arrives

#define VW_MAX_MESSAGE_LEN 40 // Same as solar/sensor
#define VW_RX_PIN 7 // PD7 (13)
#define RF_POWER_PIN 8 // PB0 (14) Toggle power to RF receiver

#define WD_DO_STUFF 8 // How many watchdog interupts before doing real work (same a sensor).




byte solar_val = 127; // Temporary half value
volatile byte wd_isr = WD_DO_STUFF;


ISR(WDT_vect)
{
  // Wake up by watchdog
  if (wd_isr == 0) {
      wd_isr = WD_DO_STUFF;
      // Stay awake until a message is received.
  } else {
      --wd_isr; 
      // Go back to sleep.
      goToSleep();
  }
}

void setup()
{
  // disable ADC
  ADCSRA = 0;
  
  // disable analog comparitor
  ACSR = (1 << ACD) | (0 << ACIE);
  
  watchdog_setup();
  
  pinMode(DEBUG_LED_PIN, OUTPUT);  
  analogWrite(DEBUG_LED_PIN, DEBUG_LED_PWM_AWAKE);
  
  pinMode(RF_POWER_PIN, OUTPUT);  
  digitalWrite(RF_POWER_PIN, HIGH);  
  
  pinMode(PWM_PIN, OUTPUT);  // TMP until dedicated chip arrives
  analogWrite(PWM_PIN, solar_val);

  Serial.begin(9600);
  Serial.println("Setup");

  vw_set_rx_pin(VW_RX_PIN);
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver running
}


void loop()
{
  // Reset watchdog so he knows all is well.
  wdt_reset();
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) { // Non-blocking
  
    // Indicate a message received
    analogWrite(DEBUG_LED_PIN, DEBUG_LED_PWM_MSG);
    
  
    int i;
    int val = 0;
    byte comma = 0;
    for (i = 0; i < buflen; i++) {

      if (buf[i] == 44) {      
        // CSV - S,88,20.17,21.91,871,5595,5436
        // comma 5 = solar panel reading
        // comma 6 = battery reading
        
        if (comma == 5) {
          solar_val = map(val, 0, 10000, 0, 255);
          
          
          // Print the solar reading
          //Serial.print(" pwm("); 
          //Serial.print(solar_val, DEC); 
          //Serial.print(")"); 
        }
        
        ++comma;
        val = 0;
      }
      
      
      Serial.write(char(buf[i]));
      
      
      if (comma == 5 || comma == 6) {
        byte c = buf[i];
        // Only interested in numbers.
        // ASCII hex values 0x30 to 0x30 are decimal 0 to 9
        if (c > 0x2F && c < 0x3A) {
            if (val > 0) {
                // Expecting decimal numbers like 24,
                // so for each new argument multiply by ten.
                val *= 10;
                val += c - 0x30;
            } else {
                val = c - 0x30;
            }
            //Serial.print(val, DEC); 
        }
      }
      
      
    }
    
    
    // Print the battery reading
    //Serial.print(" Bat("); 
    //Serial.print(val, DEC); 
    //Serial.print(")");
    
    
    Serial.println(); 
    
    // Ofcourse this is rather pointless a sleep will kill the pwm (replacing with digital pot)
    analogWrite(PWM_PIN, solar_val);
    
    // Turn off message indicator
    analogWrite(DEBUG_LED_PIN, DEBUG_LED_PWM_AWAKE);
    
    // Wait for the serial data to be sent
    Serial.flush();
    
    // Sleep 'till the next message is due
    goToSleep(); 
  }

}


/**
 * Watchdog for while awake to ensure things are ticking over.
 */
void watchdog_setup()
{
  // Clear any previous watchdog interupt
  MCUSR = 0;

  // WD interrupt after 8 seconds, 
  // unless wdt_reset(); has been successfully called
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; // 8.0 seconds 
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
  wdt_reset();
}

void goToSleep()
{

  // Turn off the RF receiver
  digitalWrite(RF_POWER_PIN, LOW);
  
  // Indicate sleep mode (not pwm as would need to not be totally asleep)
  digitalWrite(DEBUG_LED_PIN, LOW);
 
  cli();
  
  // ensure ADC is off
  ADCSRA = 0;
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  power_all_disable();

  sleep_enable();
  sei();
  
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
  MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
  sleep_cpu();              // sleep within 3 clock cycles of above
                      
  sleep_disable();  
  MCUSR = 0; // clear the reset register 
  
  power_all_enable(); 

  // turn on the RF receiver
  digitalWrite(RF_POWER_PIN, HIGH);
  
  // Indicate awake
  analogWrite(DEBUG_LED_PIN, DEBUG_LED_PWM_AWAKE);
  
} 
