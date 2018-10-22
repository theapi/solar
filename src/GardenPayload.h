/**
 *  Library for defining the payload datastructure the garden sensor.
 */
#ifndef THEAPIGARDENPAYLOAD_h
#define THEAPIGARDENPAYLOAD_h

#include <stdint.h>

namespace theapi {

  class GardenPayload {
    public:
      typedef struct {
          uint8_t msg_type;
          uint8_t msg_id;
          int16_t a;
          int16_t b;
          int16_t c;
          int16_t d;
          int16_t e;
          int16_t f;
          // The radio signal stats.
          int16_t rssi;
          int16_t snr;
          int16_t frq_error;
      } payload_t;

      const static uint8_t SIZE = 20;
      const static uint8_t DATA_SIZE = 14;

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
      uint16_t getLight();
      void setLight(uint16_t val);

      // The sith integer data
      int16_t getCpuTemperature();
      void setCpuTemperature(int16_t val);

      // The seventh integer data
      int16_t getTemperature();
      void setTemperature(int16_t val);

      /**
       * The most recent RSSI (Receiver Signal Strength Indicator)
       */
      int16_t getRssi();
      void setRssi(int16_t val);

      /**
       * The Signal-to-noise ratio (SNR) of the last received message,
       * as measured by the receiver.
       */
      int16_t getSnr();
      void setSnr(int16_t val);

      /**
       * The LoRa receiver estimates the frequency offset between the
       * receiver centre frequency and that of the received LoRa signal.
       * This function returns the estimates offset (in Hz) of the last received message.
       */
      int16_t getFreqError();
      void setFreqError(int16_t val);

      // Creates a byte array for sending via the radio
      void serialize(uint8_t payload[GardenPayload::SIZE]);

      // Parse the read byte data from the radio
      void unserialize(uint8_t payload[GardenPayload::SIZE]);

    private:
      payload_t _payload;

  };

}

#endif
