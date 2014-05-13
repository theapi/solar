#include <Wire.h>

const byte SLAVE_ADDRESS = 100;  // which address debugging goes to


  #define BEGIN_DEBUG   Wire.begin ()
  #define TRACE(x)      I2Cdebug.print   (x)
  #define TRACE2(x,y)   I2Cdebug.print   (x,y)
  #define TRACELN(x)    I2Cdebug.println (x)
  #define TRACELN2(x,y) I2Cdebug.println (x,y)
  
  class tI2Cdebug : public Print
  {
  public:
    virtual size_t write (const byte c)  { 
      Wire.beginTransmission (SLAVE_ADDRESS);
      Wire.write (c);
      Wire.endTransmission ();
    }  // end of tI2Cdebug::write
  }; // end of tI2Cdebug
      
  // an instance of the I2Cdebug object
  tI2Cdebug I2Cdebug;


long counter;
unsigned long start;

void setup() {
  start = micros ();

  BEGIN_DEBUG;
  TRACELN ("Commenced device-under-test debugging!");
}  // end of setup

void loop() 
{

  counter++;
  if (counter == 100000)
  {
    TRACELN ("100000 reached.");
    TRACE ("took ");
    TRACE (micros () - start);
    TRACE (" in HEX is: ");
    TRACELN2 (micros () - start, HEX);
  
    counter = 0;
  }  // end of if

}  // end of loop
