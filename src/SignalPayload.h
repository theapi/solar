/**
 * Info about the radio signal.
 */
#ifndef THEAPISIGNALPAYLOAD_h
#define THEAPISIGNALPAYLOAD_h

#include <stdint.h>

namespace theapi {
  class SignalPayload
  {
    public:
      typedef struct{
        uint8_t msg_type;
        uint8_t msg_id;
        int16_t rssi;
        int16_t snr;
        int16_t frq_error;
      }
      payload_t;

      const static uint8_t SIZE = 8;

      SignalPayload();

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

      /**
       * The most recent RSSI (Receiver Signal Strength Indicator)
       */
      int16_t getRssi();
      void setRssi(int16_t);

      /**
       * The Signal-to-noise ratio (SNR) of the last received message,
       * as measured by the receiver.
       */
      int16_t getSnr();
      void setSnr(int16_t);

      /**
       * The LoRa receiver estimates the frequency offset between the
       * receiver centre frequency and that of the received LoRa signal.
       * This function returns the estimates offset (in Hz) of the last received message.
       */
      int16_t getFrqError();
      void setFrqError(int16_t);

      // Creates the byte array for sending.
      void serialize(uint8_t payload[SignalPayload::SIZE]);

      // Parse the serialized byte data.
      void unserialize(uint8_t payload[SignalPayload::SIZE]);

    private:
      payload_t _payload;

  };
};
#endif
