/*
  Payload.h - Library for defining the payload datastructure for the Solar network.

  Created by Peter Clarke.
*/
#ifndef Payload_h
#define Payload_h

#include <stdint.h>

#define Payload_SIZE 14

class Payload
{
  public:
    typedef struct{
      uint8_t msg_id;
      uint8_t device_id;
      uint16_t a;
      uint16_t b;
      uint16_t c;
      uint16_t d;
      uint16_t e;
      uint16_t f;
    }
    payload_t;

    Payload();

    // How big the radion payload is.
    uint8_t getPayloadSize();

    // The id of the device.
    uint8_t getDeviceId();
    void setDeviceId(uint8_t device_id);

    // The id, not neccessarily unique, of the message.
    uint8_t getMsgId();
    void setMsgId(uint8_t msg_id);


    // The first integer data
    uint16_t getA();
    void setA(uint16_t val);

    // The second integer data
    uint16_t getB();
    void setB(uint16_t val);

    // The third integer data
    uint16_t getC();
    void setC(uint16_t val);

    // The forth integer data
    uint16_t getD();
    void setD(uint16_t val);
    
    // The sith integer data
    uint16_t getE();
    void setE(uint16_t val);
    
    // The seventh integer data
    uint16_t getF();
    void setF(uint16_t val);

    // Creates a byte array for sending via the radio
    void serialize(uint8_t payload[Payload_SIZE]);

    // Parse the read byte data from the radio
    void unserialize(uint8_t payload[Payload_SIZE]);

  private:
    payload_t _payload;

};

#endif
