/**
 *  Library for defining the payload datastructure the ping payload.
 */
#ifndef THEAPIPAYLOADPING_h
#define THEAPIPAYLOADPING_h

#include <stdint.h>

#define TheapiPayloadPing_SIZE 2

class TheapiPayloadPing
{
  public:
    typedef struct{
      uint8_t msg_type;
      uint8_t msg_id;
    }
    payload_t;

    TheapiPayloadPing();

    // How big the payload is.
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

    // Creates a byte array for sending via the radio
    void serialize(uint8_t payload[TheapiPayloadPing_SIZE]);

    // Parse the read byte data from the radio
    void unserialize(uint8_t payload[TheapiPayloadPing_SIZE]);

  private:
    payload_t _payload;

};

#endif
