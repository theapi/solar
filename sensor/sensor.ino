#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <VirtualWire.h>
#include <math.h>

#define LED_DEBUG 13

#define PIN_TX 7         // PD7
#define PIN_TX_POWER 8   // PD8


#define PIN_SENSOR_POWER 9       // PB1 (used for all sensors)
#define PIN_SENSOR_THERMISTOR A0 // PC0
#define PIN_SENSOR_SOIL A1       // PC1
#define PIN_SENSOR_LM35 A2       // PC1

#define PIN_SOLAR A5 // PC5


#define SLEEP_FLAG_STAYAWAKE 0  // Dont sleep
#define SLEEP_FLAG_IDLE 1       // Gone to sleep because nothing to do
#define SLEEP_FLAG_DARK 2       // Gone to sleep because it is dark
#define AWAKE_TX_MAX 3 // How many transmissions before sleeping


#define WD_DO_STUFF 8 // How many watchdog interupts before doing real work.

// The analoge value of the ldr
int ldr;
byte sleep_flag;
byte msgId;
byte awake_tx_count;

// When the last transmit was attempted.
unsigned long tx_last = 0; 
// The transmit interval.
const long tx_interval = 1000; 

volatile byte wd_isr = WD_DO_STUFF;

/**
 * ISR for INT0
 */
void wakeUp() 
{
  // Just wake up
  detachInterrupt(0); // No wakeup interupt while awake
}

ISR(WDT_vect)
{
  // Wake up by watchdog
  if (wd_isr == 0) {
      wd_isr = WD_DO_STUFF;
  } else {
      --wd_isr; 
      // Go back to sleep.
      goToSleep(SLEEP_FLAG_IDLE);
  }
}

void setup()
{
    watchdog_setup();
    
    //Serial.begin(9600); 
    //pinMode(LED_DEBUG, OUTPUT); 
    
    pinMode(PIN_TX, OUTPUT); 
    pinMode(PIN_TX_POWER, OUTPUT);
    pinMode(PIN_SENSOR_POWER, OUTPUT);

    digitalWrite(LED_DEBUG, HIGH);
    
    // VirtualWire
    vw_setup(2000);      // Bits per sec
    vw_set_tx_pin(PIN_TX);

}

void loop()
{
    // Reset watchdog so he knows all is well.
    wdt_reset();
        
    unsigned long now = millis();   
    // Transmit after interval
    if (now - tx_last >= tx_interval) {
        tx_last = now;

        digitalWrite(PIN_SENSOR_POWER, HIGH);
        long vcc = readVcc();
        int solar_read = analogRead(PIN_SOLAR);
        int thermistor_read = analogRead(PIN_SENSOR_THERMISTOR);
        int soil_read = analogRead(PIN_SENSOR_SOIL);
        int lm35_read = analogRead(PIN_SENSOR_LM35);
        digitalWrite(PIN_SENSOR_POWER, LOW);

        double temperature_external = Thermistor(thermistor_read);
        double temperature_internal = (vcc/1000.0 * lm35_read * 100.0)/1024.0;
        int solr_mv = vcc / 1024.0 * solar_read * 2;
        
        char temp_internal[6];
        dtostrf(temperature_internal, 1, 2, temp_internal);

        char temp_external[6];
        dtostrf(temperature_external, 1, 2, temp_external);
  
        // Send a transmission
        digitalWrite(PIN_TX_POWER, HIGH);
        // int = 5 bytes in transmision string
        // long int = 10 bytes in transmision string
        // byte = 3 bytes in transmision string
        // temperature string 6 bytes (or is it 5 because the null terminator gets removed?)
        char msg[40]; // string to send
        sprintf(msg, "S,%d,%s,%s,%i,%i,%lu", msgId, temp_internal, temp_external, soil_read, solr_mv, vcc);
        //Serial.println(msg); 
        vw_send((uint8_t *)msg, strlen(msg));
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(PIN_TX_POWER, LOW);
        
        
        ++msgId;
        ++awake_tx_count;
        if (awake_tx_count > AWAKE_TX_MAX) {
            awake_tx_count = 0;
            // Watchdog will wake us up in 8 seconds time.
            goToSleep(SLEEP_FLAG_IDLE);
        }        
    }
}

/**
 * @see http://playground.arduino.cc/ComponentLib/Thermistor2
 */
double Thermistor(int RawADC) {   
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1))); 
  //         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
 
 return Temp;
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
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
  //wdt_enable(WDTO_8S);
  wdt_reset();
}

void goToSleep(byte flag)
{
  sleep_flag = flag;
  
  //digitalWrite(LED_DEBUG, LOW);
  
  
  cli();
  
  // disable ADC
  byte old_ADCSRA = ADCSRA;
  ADCSRA = 0;
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  if (sleep_flag == SLEEP_FLAG_DARK) {
    // Turn off watchdog
    //wdt_disable();
    // Attach the external interupt to the LDR.
    //attachInterrupt(0, wakeUp, HIGH);
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
    watchdog_setup(); // watchdog back on
  }
  
  power_all_enable();    // power everything back on
  
  // put ADC back
  ADCSRA = old_ADCSRA;
  
  //digitalWrite(LED_DEBUG, HIGH);
  
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

