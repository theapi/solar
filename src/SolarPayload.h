/**
 *  Library for defining the payload datastructure for a solar sensor.
 */
#ifndef THEAPISOLARPAYLOAD_h
#define THEAPISOLARPAYLOAD_h

#include <stdint.h>

namespace theapi {

  class SolarPayload {
    public:
      typedef struct {
        uint8_t MessageType;
        uint16_t DeviceId;
        uint8_t MessageId;
        uint16_t VCC;
        uint16_t ChargeMv;
        int16_t ChargeMa;
        uint16_t Light;
        int16_t CpuTemperature;
        int16_t Temperature;
      } payload_t;

      const static uint8_t SIZE = 16;

      SolarPayload();

      // How big the payload is.
      uint8_t size();

      /**
       * The type of message.
       * Each Theapi payload has a message type,
       * so the receiver can read the first byte to know which type is arriving.
       */
      uint8_t getMsgType();
      void setMsgType();

      // The id of the device.
      uint16_t getDeviceId();
      void setDeviceId(uint16_t device_id);

      // The id, not neccessarily unique, of the message.
      uint8_t getMsgId();
      void setMsgId(uint8_t msg_id);

      uint16_t getVcc();
      void setVcc(uint16_t val);

      uint16_t getChargeMv();
      void setChargeMv(uint16_t val);

      int16_t getChargeMa();
      void setChargeMa(int16_t val);

      uint16_t getLight();
      void setLight(uint16_t val);

      // The sith integer data
      int16_t getCpuTemperature();
      void setCpuTemperature(int16_t val);

      // The seventh integer data
      int16_t getTemperature();
      void setTemperature(int16_t val);

      // Creates a byte array for sending via the radio
      void serialize(uint8_t buffer[SolarPayload::SIZE]);

      // Parse the read byte data from the radio
      void unserialize(uint8_t buffer[SolarPayload::SIZE]);

    private:
      payload_t _payload;

  };

}

#endif
