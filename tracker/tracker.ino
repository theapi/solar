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

int lightPin0 = 0;  //define a pin for Photo resistor
int lightPin1 = 1;

void setup()
{
    Serial.begin(9600); 
}

void loop()
{
    Serial.print(analogRead(lightPin0)); 
    Serial.print(':');
    Serial.print(analogRead(lightPin1));
    Serial.println(); 
    delay(100); 
}
