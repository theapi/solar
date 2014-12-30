/**
 Receive the sensor's transmission, then display it on the analogue meter.
 */

#include <Wire.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management
#include <VirtualWire.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Nrf24Payload.h>

#define DEVICE_ID 'E'
 
//#define RX_ADDRESS "AAAAA"
//#define RX_ADDRESS "BBBBB"
//#define RX_ADDRESS "CCCCC"
//#define RX_ADDRESS "DDDDD"
#define RX_ADDRESS "EEEEE"


#define BASE_ADDRESS "1BASE"

#define DEBUG_LED_PIN 5 // PD5 (11) - Red led button
#define DEBUG_LED_PWM_AWAKE 127 // pwm value to show contoller is awake
#define DEBUG_LED_PWM_MSG 255 // pwm value to show message received


// For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
// For MCP4725A0 the address is 0x60 or 0x61
// For MCP4725A2 the address is 0x64 or 0x65
#define DAC_ADDRESS      (0x62)
#define DAC_CMD_WRITEDAC (0x40)  // Writes data to the DAC

#define VW_MAX_MESSAGE_LEN 40 // Same as solar/sensor
#define VW_RX_PIN 7 // PD7 (13)
#define RF_POWER_PIN 8 // PB0 (14) Toggle power to RF receiver

#define PIN_CE  9
#define PIN_CSN 10

#define WD_DO_STUFF 8 // How many watchdog interupts before doing real work (same a sensor).


int solar_val = 3685; // 12bit DAC (max 4095 = 0xFFF)
volatile byte wd_isr = WD_DO_STUFF;

// The address that this node listens on
byte address[6] = RX_ADDRESS;
byte address_base[6] = BASE_ADDRESS;

RF24 radio(PIN_CE, PIN_CSN);

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
  Serial.begin(9600);
  
  // disable ADC
  ADCSRA = 0;
  
  // disable analog comparitor
  ACSR = (1 << ACD) | (0 << ACIE);
  
  
  watchdog_setup();
  
  // Setup and configure nrf24 radio
  radio.begin(); // Start up the radio
  radio.setPayloadSize(Nrf24Payload_SIZE);               
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(3,15); //  delay between retries & number of retries

  // Pipe for talking to the base
  radio.openWritingPipe(address_base);
  
  pinMode(DEBUG_LED_PIN, OUTPUT);  
  analogWrite(DEBUG_LED_PIN, DEBUG_LED_PWM_AWAKE);
  
  pinMode(RF_POWER_PIN, OUTPUT);  
  digitalWrite(RF_POWER_PIN, HIGH);  
  

  setVoltage(solar_val);


  vw_set_rx_pin(VW_RX_PIN);
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver running
  
  
  Serial.println("Setup");
  Serial.print("Size of payload = ");
  Serial.println(radio.getPayloadSize());
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
    
/*
  
    int i;
    int val = 0;
    byte comma = 0;
    for (i = 0; i < buflen; i++) {

      if (buf[i] == 44) {      
        // CSV - S,88,20.17,21.91,871,5595,5436
        // comma 5 = solar panel reading
        // comma 6 = battery reading
        
        if (comma == 5) {
          parsed_val = val;
          solar_val = map(val, 0, 10000, 0, 4095); // 0xFFF 12 bit DAC
          
          
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
        
    Serial.print(parsed_val);
    Serial.print(" -> ");
    Serial.println(solar_val); 
*/
 


    // Parse the received message into a nrf24 payload
    Nrf24Payload payload = parse433Message(buf);
    // Forward the message through the nrf24 radio
    uint8_t tx[Nrf24Payload_SIZE];
    payload.serialize(tx);
    radio.write( &tx, Nrf24Payload_SIZE);

    // Set the needle on the analor meter
    solar_val = map(payload.getD(), 0, 10000, 0, 4095); // 0xFFF 12 bit DAC
    setVoltage(solar_val);
    
    // Output for the bluetooth module
    for (int i = 0; i < buflen; i++) {
      Serial.write(char(buf[i]));
    }
    // Wait for the serial data to be sent
    Serial.flush();

    // Turn off message indicator
    analogWrite(DEBUG_LED_PIN, DEBUG_LED_PWM_AWAKE);
    

    // Sleep 'till the next message is due
    goToSleep(); 
    
    /*
    if (solar_val > 0) {
      goToSleep(SLEEP_MODE_PWR_SAVE); 
    } else {
      // no need for pwm so full sleep
      goToSleep(SLEEP_MODE_PWR_DOWN); 
    }
    */
    
  }

}

/**
 * The human hearing range is commonly given as 20 to 20,000 Hz
 *
 * @see http://playground.arduino.cc/Code/PwmFrequency
 * @see http://playground.arduino.cc/Main/TimerPWMCheatsheet
 * @see http://www.letsmakerobots.com/content/changing-pwm-frequencies-arduino-controllers
 */
 /*
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
*/

/*
void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(POT_CHIP_SELECT, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(POT_CHIP_SELECT, HIGH); 
}
*/

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
  
  // Turn on the nrf24 radio
  radio.powerDown();
  
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
  
  /*
  power_timer0_enable();
  power_timer1_enable();
  //power_timer2_enable();
  power_usart0_enable();
  power_twi_enable();
  */
  power_all_disable();
  
  // Turn on the nrf24 radio
  radio.powerUp();
  

  // turn on the RF receiver
  digitalWrite(RF_POWER_PIN, HIGH);
  
  // Indicate awake
  analogWrite(DEBUG_LED_PIN, DEBUG_LED_PWM_AWAKE);
  
} 

void setVoltage(int output)
{
  Wire.beginTransmission(DAC_ADDRESS);
  Wire.write(DAC_CMD_WRITEDAC);
  Wire.write(output / 16);                   // Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
  Wire.write((output % 16) << 4);            // Lower data bits          (D3.D2.D1.D0.x.x.x.x)
  Wire.endTransmission();
}

Nrf24Payload parse433Message(uint8_t buf[VW_MAX_MESSAGE_LEN])
{
  // Convert the uint8_t buffer into a char buffer for strtok to use.
  char buffer[VW_MAX_MESSAGE_LEN];
  for (byte j=0; j<VW_MAX_MESSAGE_LEN; j++) {
    buffer[j] = buf[j];
  }
  
  Nrf24Payload payload = Nrf24Payload();
  
  payload.setDeviceId(DEVICE_ID);
  
  // Expects a csv string
  // eg; S,88,20.17,21.91,871,5595,5436
  // from: sprintf(msg, "S,%d,%s,%s,%i,%i,%lu", msgId, temp_internal, temp_external, soil_read, solr_mv, vcc);
  // NB rounding the temperature floats to ints since they're not that accurate anyway.
  
  int i = 0;
  char *token;
  token = strtok(buffer, ",");
  while (token != NULL) {
    printf("%i: %s\n", i, token);
    switch (i) {
      case 0:
        payload.setType('S');
        break;
      case 1:
        // message id
        payload.setId(atoi(token));
        break;
      case 2:
        // temp_internal
        payload.setA(atoi(token));
        break;
      case 3:
        // temp_external
        payload.setB(atoi(token));
        break;
      case 4:
        // soil_read
        payload.setC(atoi(token));
        break;
      case 5:
        // solr_mv
        payload.setD(atoi(token));
        break;
      case 6:
        // vcc
        payload.setVcc(atoi(token));
        break;
    }
    i++;
    token = strtok(NULL, ",");
  }
  
  return payload;
}

