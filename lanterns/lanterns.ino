/*
 Lanterns

ATTINY85
 */
 
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management


#define WD_DO_STUFF 8 // How many watchdog interupts before doing real work.

const byte debug_pin = 1;
const byte leds = 0;  // PB0 the led string 
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

volatile byte wd_isr = WD_DO_STUFF;

/**
 * ISR for INT0
 */
void wakeUp() 
{
  // Just wake up
}

ISR(WDT_vect)
{
  // Wake up by watchdog
  if (wd_isr == 0) {
      wd_isr = WD_DO_STUFF;
  } else {
      --wd_isr; 
      // Go back to sleep.
      //goToSleep();
  }
}


void setup()  
{ 
  
  watchdog_setup();
  pinMode(leds, OUTPUT);
  pinMode(debug_pin, OUTPUT);
  digitalWrite(debug_pin, HIGH);
} 


void loop()  
{ 
  
  // Reset watchdog so he knows all is well.
  wdt_reset();
  
 
  
  // set the brightness
  analogWrite(leds, brightness);    
  


  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;
  
  if (brightness == 0) {
    goToSleep();
  }

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
    

  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);                            
}

/**
 * Watchdog for while awake to ensure things are ticking over.
 */
void watchdog_setup()
{
  // Clear any previous watchdog interupt
  MCUSR = 0;

  
  wdt_enable(WDTO_8S);
  wdt_reset();
}

void goToSleep()
{

  
  digitalWrite(debug_pin, LOW);
  
  
  cli();
  
  // disable ADC
  byte old_ADCSRA = ADCSRA;
  ADCSRA = 0;
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);


  power_all_disable();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sei();
  
  // turn off brown-out enable in software
  //MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
  //MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
  sleep_cpu();              // sleep within 3 clock cycles of above

                              
  sleep_disable();  
  MCUSR = 0; // clear the reset register 
  
  
  power_all_enable();    // power everything back on
  
  // put ADC back
  ADCSRA = old_ADCSRA;
  
  digitalWrite(debug_pin, HIGH);
  
} 

