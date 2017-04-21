/**
 * Library for defining the payload datastructures.
 */
#ifndef THEAPIACKPAYLOAD_h
#define THEAPIACKPAYLOAD_h

#include <stdint.h>

namespace theapi {
  class AckPayload
  {
    public:
      typedef struct{
        uint8_t msg_type;
        uint8_t msg_id;
      }
      payload_t;

      const static uint8_t SIZE = 2;

      AckPayload();

      // How big the radion payload is.
      uint8_t size();

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
      void serialize(uint8_t payload[AckPayload::SIZE]);

      // Parse the read byte data from the radio
      void unserialize(uint8_t payload[AckPayload::SIZE]);

    private:
      payload_t _payload;

  };
};
#endif
