#include <Wire.h>

const byte I2C_SLAVE_ADDRESS = 100;  // which address messages goes to

/*
  #define TRACE(x)      I2Cdebug.print   (x)
  #define TRACE2(x,y)   I2Cdebug.print   (x,y)
  #define TRACELN(x)    I2Cdebug.println (x)
  #define TRACELN2(x,y) I2Cdebug.println (x,y)
*/

// Extend the print class for use with i2c
class tI2C : public Print
{
  public:
    virtual size_t write (const byte c)  { 
      Wire.beginTransmission (I2C_SLAVE_ADDRESS);
      Wire.write (c);
      Wire.endTransmission ();
    }
};
      
// an instance of the I2C object
tI2C I2C;


long counter;
unsigned long start;

void setup() 
{
  start = micros();
  Wire.begin();
}  

void loop() 
{
  counter++;
  if (counter == 100000) {
    I2C.println("100000 reached.");
    I2C.print("took ");
    I2C.print(micros() - start);
    I2C.print(" in HEX is: ");
    I2C.println(micros() - start, HEX);
  
    counter = 0;
  }

} 
