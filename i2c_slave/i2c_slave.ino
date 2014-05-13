#include <Wire.h>

const byte MY_ADDRESS = 100;

char buf [1000];
volatile int inpoint, outpoint;

void setup (void)
{
  Serial.begin (115200);   // debugging

  Serial.println ();
  Serial.println ("Commencing debugging session ...");
  Serial.println ();

  Wire.begin (MY_ADDRESS);
  Wire.onReceive (receiveEvent);

}  // end of setup


void receiveEvent (int howMany)
{
  while (Wire.available () > 0)
  {
    char c = Wire.read ();
    int next = inpoint + 1;  // next insert point

    // wrap-around at end of buffer
    if (next >= sizeof buf)
      next = 0;

    if (next == outpoint)  // caught up with removal point?
      continue;  // give up

    // insert at insertion point
    buf [inpoint] = c;
    inpoint = next;  // advance to next
  }  // end of while available
}  // end of receiveEvent


void loop (void)
{
  // insertion and removal point the same, nothing there
  noInterrupts ();  // atomic test of a 16-bit variable
  if (outpoint == inpoint)
    {
    interrupts ();
    return;
    }
  interrupts ();

  // display anything found in the circular buffer
  Serial.print (buf [outpoint]);

  noInterrupts ();
  if (++outpoint >= sizeof buf)
    outpoint = 0;  // wrap around
  interrupts ();

}  // end of loop
