

#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>
#include <avr/sleep.h>    // Sleep Modes
#include <avr/power.h>    // Power management

#define RNG_WATCHDOG 0 // and remove ISR(WDT_vect) from RNG.cpp (line 195) 

#include <Crypto.h>
#include <AES.h>
#include <RH_RF95.h>
#include <Adafruit_ADS1015.h>

#include "config.h"
#include "GardenPayload.h"
#include "AckPayload.h"

#define ENCRYPTION_BUFFER_SIZE 16

#define RFM95_CS 10
//#define RFM95_RST 2
#define RFM95_INT 3

#define RF95_FREQ 868.0

#define PIN_SENSOR_GND 8
#define PIN_SENSOR_VCC 7
#define PIN_SENSOR_SOIL A2
// Data wire is plugged into pin 6 on the Arduino
#define ONE_WIRE_BUS 6

#define WD_DO_STUFF 1 // How many watchdog interupts before doing real work.

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// The external I2C ADC.
Adafruit_ADS1015 ads;


theapi::GardenPayload tx_payload = theapi::GardenPayload();
theapi::AckPayload ack_payload = theapi::AckPayload();

uint8_t msg_id = 0;
int rssi = 0;

AES128 cipher;
uint8_t encrypted_buffer[ENCRYPTION_BUFFER_SIZE];
uint8_t decrypted_buf[ENCRYPTION_BUFFER_SIZE];

// Setup a oneWire instance to communicate with any OneWire devices.
OneWire oneWire(ONE_WIRE_BUS);
// Pass the oneWire reference to Dallas Temperature.
DallasTemperature onewireSensors(&oneWire);
DeviceAddress deviceAddress;

volatile byte wd_isr = WD_DO_STUFF;

ISR(WDT_vect) {
  // Wake up by watchdog
  if (wd_isr == 0) {
      wd_isr = WD_DO_STUFF;
  } else {
      --wd_isr; 
      // Go back to sleep.
      goToSleep();
  }
}

void setup() {
  watchdog_setup();
  Serial.begin(115200);

  ads.setGain(GAIN_TWO); // 2x gain   +/- 2.048V  1 bit = 1mV
  
  delay(100);

  while (!rf95.init()) {
    Serial.println("init failed");
    while (1);
  }
  Serial.println("init OK");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  rf95.setTxPower(10, false);

  pinMode(PIN_SENSOR_VCC, OUTPUT);
  digitalWrite(PIN_SENSOR_VCC, HIGH);

  // Begin the onwire library. 
  onewireSensors.setWaitForConversion(true);
  onewireSensors.begin();
  onewireSensors.getAddress(deviceAddress, 0);

  // Connect the light sensor to ground.
  // The light sensor is a voltage source.
  // So to insure that there is not a voltage higher than vcc
  // on the external ADC, it's ground is connected via the pin
  // which is high impedance on startup.
  pinMode(PIN_SENSOR_GND, OUTPUT);
  digitalWrite(PIN_SENSOR_GND, LOW);

  // Begin the external ADC.
  ads.begin();
}

void loop() {

  // Reset watchdog so he knows all is well.
  wdt_reset();

  // Power up the sensors.
  digitalWrite(PIN_SENSOR_VCC, HIGH);

  tx_payload.setMsgId(++msg_id);

  uint16_t vcc = readVcc();
  tx_payload.setVcc(vcc);

  // Read the temperature.
  tx_payload.setTemperature(readTemperature());

  tx_payload.setChargeMa(readSolarCurrent());
  tx_payload.setChargeMv(readSolarVolts());
  tx_payload.setLight(readLight(vcc));
  tx_payload.setSoil(readSoil(vcc));

  // Power down the soil sensor.
  digitalWrite(PIN_SENSOR_VCC, LOW);

  // Build the payload buffer.
  uint8_t payload_buf[tx_payload.size()];
  tx_payload.serialize(payload_buf);
  // Encrypt the payload.
  cipher.encryptBlock(encrypted_buffer, payload_buf);
  // Send it.
  rf95.send(encrypted_buffer, ENCRYPTION_BUFFER_SIZE);
  rf95.waitPacketSent();
  
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(2000)) { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len)) {
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("RSSI: ");
      rssi = rf95.lastRssi();
      Serial.println(rssi, DEC);    

      uint8_t decrypted_ack_buf[16];
      cipher.decryptBlock(decrypted_ack_buf, buf);
      ack_payload.unserialize(decrypted_ack_buf);

    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply");
  }
  
  // Watchdog will wake us up in 8 seconds time.
  goToSleep();
}

/**
 * Watchdog for while awake to ensure things are ticking over.
 */
void watchdog_setup() {
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

void goToSleep() {
  //digitalWrite(LED_DEBUG, LOW);
  
 
  cli();
  
  // disable ADC
  byte old_ADCSRA = ADCSRA;
  ADCSRA = 0;
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  power_all_disable();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sei();
  
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);  // turn on brown-out enable select
  MCUCR = bit (BODS);        // this must be done within 4 clock cycles of above
  sleep_cpu();              // sleep within 3 clock cycles of above
                              
  sleep_disable();  
  MCUSR = 0; // clear the reset register 
  
  power_all_enable();    // power everything back on
  
  // put ADC back
  ADCSRA = old_ADCSRA;
  
  //digitalWrite(LED_DEBUG, HIGH);
  
} 

/**
 * Read the internal voltage.
 */
uint16_t readVcc() 
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
 
  uint16_t result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

uint16_t readSolarCurrent() {
  // 1 ohm inline resistor
  // Invert result as the wires are connected goofy.
  return ads.readADC_Differential_0_1() * -1;
}

uint16_t readSolarVolts() {
  // Voltage divider to reduce the maximum to 2V.
  // 8V -> 2V
  // Solar panel --- 300K --- | --- 100K --- GND
  int16_t val = ads.readADC_SingleEnded(2);
  // Calibration:
  // 1000mV = 268, 2000mV = 537, 3000mV = 805... 
  // so 3.7313mV = 1
  return val * 3.7313F;
}

uint16_t readLight(uint16_t vcc) {
  // A very small solar panel with no load.
  // Mini solar panel --- 100K --- | --- 100K --- GND
  int16_t val = ads.readADC_SingleEnded(3);
  return val * 2;
}

uint16_t readSoil(uint16_t vcc) {
  uint16_t val;

  // This first reading after a mux change is unreliable.
  val = analogRead(PIN_SENSOR_SOIL);

  // Average the next 2 readings.
  val = 0;
  for (uint8_t i = 0; i < 2; i++) { 
    val += analogRead(PIN_SENSOR_SOIL);
  }

  return val / 2;
}

uint16_t readTemperature() {
  // Send the command to get temperatures
  onewireSensors.requestTemperatures(); 
  return onewireSensors.getTempCByIndex(0);
}

