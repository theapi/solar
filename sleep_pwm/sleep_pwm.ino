
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
  
  // Try a low frequency pwm.
  setPwmFrequency(256);
  
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

/**
 * The human hearing range is commonly given as 20 to 20,000 Hz
 *
 * @see http://playground.arduino.cc/Code/PwmFrequency
 * @see http://playground.arduino.cc/Main/TimerPWMCheatsheet
 * @see http://www.letsmakerobots.com/content/changing-pwm-frequencies-arduino-controllers
 */
void setPwmFrequency(int divisor) 
{
  // NB: Half these frequencies as using internal 8mhz clock.
  //TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz

  byte mode;
  switch(divisor) {
    case 1: mode = 0x01; break;
    case 8: mode = 0x02; break;
    case 32: mode = 0x03; break;
    case 64: mode = 0x04; break;
    case 128: mode = 0x05; break;
    case 256: mode = 0x06; break;
    case 1024: mode = 0x7; break;
    default: return;
  }
  TCCR2B = TCCR2B & 0b11111000 | mode;
}


