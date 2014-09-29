
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management

#define DEBUG_LED_PIN 13
#define PWM_PIN 11 // timer 2
#define WD_DO_STUFF 1 // How many watchdog interupts before doing real work

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
  
  power_spi_disable();
  power_twi_disable();
  
  
  watchdog_setup();
  
  pinMode(DEBUG_LED_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  
  analogWrite(PWM_PIN, 128);   
} 

void loop()
{
  // Reset watchdog so he knows all is well.
  wdt_reset();
  
  // wait, then sleep
  delay(2000); 
  
  goToSleep(); 
}

/**
 * Watchdog to wake up every 8 seconds
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

  // Indicate sleeping
  digitalWrite(DEBUG_LED_PIN, LOW);
 
  cli();
  
  // ensure ADC is off
  ADCSRA = 0;
  
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  //power_all_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_usart0_disable();
  
  // leaving timer2 on for the pwm

  sleep_enable();
  sei();
  
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
  MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
  sleep_cpu();              // sleep within 3 clock cycles of above
                      
  sleep_disable();  
  MCUSR = 0; // clear the reset register 
  
  //power_all_enable(); 
  power_timer0_enable();
  power_timer1_enable();
  power_usart0_enable();

  // Indicate awake
  digitalWrite(DEBUG_LED_PIN, HIGH);
  
} 

