#include "SignalPayload.h"
#include "Payload.h"

namespace theapi {

  SignalPayload::SignalPayload() {
    setMsgType();
    _payload.msg_id = 0;
  }

  uint8_t SignalPayload::size() {
    return SIZE;
  }

  void SignalPayload::setMsgType() {
    _payload.msg_type = theapi::Payload::SIGNAL;
  }

  uint8_t SignalPayload::getMsgType() {
    return _payload.msg_type;
  }

  // The id, not neccessarily unique, of the message.
  uint8_t SignalPayload::getMsgId()
  {
    return _payload.msg_id;
  }

  void SignalPayload::setMsgId(uint8_t id)
  {
    _payload.msg_id = id;
  }

  int16_t SignalPayload::getRssi()
  {
    return _payload.rssi;
  }

  void SignalPayload::setRssi(int16_t val)
  {
    _payload.rssi = val;
  }

  int16_t SignalPayload::getSnr()
  {
    return _payload.snr;
  }

  void SignalPayload::setSnr(int16_t val)
  {
    _payload.snr = val;
  }

  int16_t SignalPayload::getFrqError()
  {
    return _payload.frq_error;
  }

  void SignalPayload::setFrqError(int16_t val)
  {
    _payload.frq_error = val;
  }

  // Populates the given array with the payload data
  void SignalPayload::serialize(uint8_t payload[SignalPayload::SIZE])
  {
    payload[0] = _payload.msg_type;
    payload[1] = _payload.msg_id;

    payload[2] = (_payload.rssi >> 8);
    payload[3] = _payload.rssi;
    payload[4] = (_payload.snr >> 8);
    payload[5] = _payload.snr;
    payload[6] = (_payload.frq_error >> 8);
    payload[7] = _payload.frq_error;
  }

  // Parse the read byte data
  void SignalPayload::unserialize(uint8_t payload[SignalPayload::SIZE])
  {
    _payload.msg_type = payload[0];
    _payload.msg_id = payload[1];

    _payload.rssi = (payload[2] << 8) | payload[3];
    _payload.snr = (payload[4] << 8) | payload[5];
    _payload.frq_error = (payload[6] << 8) | payload[7];
  }

}
