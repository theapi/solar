/**
 * Library for defining the payload datastructures.
 */
#ifndef THEAPIPAYLOAD_h
#define THEAPIPAYLOAD_h

#include <stdint.h>

#define TheapiPayload_SIZE 14

// Message types
enum TheapiMsgTypes {
  MSG_TYPE_GENERIC = 0x35,
  MSG_TYPE_PING = 0x46,
  MSG_TYPE_GARDEN = 0x49,
  MSG_TYPE_CMD = 0x11, // ASCII DC1 Device control 1
  MSG_TYPE_ACK = 0x06,
};

class TheapiPayload
{
  public:
    typedef struct{
      uint8_t msg_type;
      uint8_t msg_id;
      int16_t a;
      int16_t b;
      int16_t c;
      int16_t d;
      int16_t e;
      int16_t f;
    }
    payload_t;

    TheapiPayload();

    // How big the radion payload is.
    uint8_t getPayloadSize();

    /**
     * The type of message.
     * Each Theapi payload has a unique message type,
     * so the receiver can read the first byte to know which type is arriving.
     */
    uint8_t getMsgType();
    void setMsgType();

    // The id, not neccessarily unique, of the message.
    uint8_t getMsgId();
    void setMsgId(uint8_t msg_id);


    // The first integer data
    int16_t getA();
    void setA(int16_t val);

    // The second integer data
    int16_t getB();
    void setB(int16_t val);

    // The third integer data
    int16_t getC();
    void setC(int16_t val);

    // The forth integer data
    int16_t getD();
    void setD(int16_t val);

    // The sith integer data
    int16_t getE();
    void setE(int16_t val);

    // The seventh integer data
    int16_t getF();
    void setF(int16_t val);

    // Creates a byte array for sending via the radio
    void serialize(uint8_t payload[TheapiPayload_SIZE]);

    // Parse the read byte data from the radio
    void unserialize(uint8_t payload[TheapiPayload_SIZE]);

  private:
    payload_t _payload;

};

#endif
