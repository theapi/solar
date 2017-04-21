/**
 *  Library for defining the payload datastructure the garden sensor.
 */
#ifndef THEAPIGARDENPAYLOAD_h
#define THEAPIGARDENPAYLOAD_h

#include <stdint.h>

namespace theapi {

  class GardenPayload {
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

      const static uint8_t SIZE = 14;

      GardenPayload();

      // How big the payload is.
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

      // The first integer data
      int16_t getVcc();
      void setVcc(int16_t val);

      // The second integer data
      int16_t getChargeMv();
      void setChargeMv(int16_t val);

      // The third integer data
      int16_t getChargeMa();
      void setChargeMa(int16_t val);

      // The forth integer data
      int16_t getLight();
      void setLight(int16_t val);

      // The sith integer data
      int16_t getSoil();
      void setSoil(int16_t val);

      // The seventh integer data
      int16_t getTemperature();
      void setTemperature(int16_t val);

      // Creates a byte array for sending via the radio
      void serialize(uint8_t payload[GardenPayload::SIZE]);

      // Parse the read byte data from the radio
      void unserialize(uint8_t payload[GardenPayload::SIZE]);

    private:
      payload_t _payload;

  };

};
#endif
