// Internal Temperature Sensor
// Example sketch for ATmega328 types.
// 
// April 2012, Arduino 1.0

void setup()
{
  Serial.begin(115200);

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC
  delay(5);

  Serial.println(F("Internal Temperature Sensor"));
  
}

void loop()
{
  // Show the temperature in degrees Celsius.
  Serial.println(mcuTemperature());
  delay(1000);
}

int mcuTemperatureRaw() {
  // Start the conversion
  ADCSRA |= _BV(ADSC);  

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  return ADCW;
}

int mcuTemperature(void)
{
  int sum = 0;
  int avg = 0;
  double t;

  // Average a bit shiftable amount of readings.
  for (uint8_t i; i < 8; i++) {
    sum += mcuTemperatureRaw();
  }
  avg = sum / 8;

  // subtract 273 for celsius
  // subtract the extra offest.
  // the 1.22 is a fixed coefficient k (http://www.atmel.com/Images/Atmel-8108-Calibration-of-the-AVR's-Internal-Temperature-Reference_ApplicationNote_AVR122.pdf)
  //@link https://playground.arduino.cc/Main/InternalTemperatureSensor
  t = (avg - 333.5) / 1.22;
  

  // The returned temperature is in degrees Celsius.
  return round(t);
}

