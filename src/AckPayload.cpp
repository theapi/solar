#include "AckPayload.h"
#include "Payload.h"

namespace theapi {

  AckPayload::AckPayload() {
    setMsgType();
    _payload.msg_id = 0;
  }

  uint8_t AckPayload::size() {
    return SIZE;
  }

  void AckPayload::setMsgType() {
    _payload.msg_type = theapi::Payload::ACK;
  }

  uint8_t AckPayload::getMsgType() {
    return _payload.msg_type;
  }

  // The id, not neccessarily unique, of the message.
  uint8_t AckPayload::getMsgId() {
    return _payload.msg_id;
  }

  void AckPayload::setMsgId(uint8_t id) {
    _payload.msg_id = id;
  }

  // Populates the given array with the payload data
  void AckPayload::serialize(uint8_t payload[AckPayload::SIZE]) {
    payload[0] = _payload.msg_type;
    payload[1] = _payload.msg_id;
  }

  // Parse the read byte data
  void AckPayload::unserialize(uint8_t payload[AckPayload::SIZE]) {
    _payload.msg_type = payload[0];
    _payload.msg_id = payload[1];
  }

}
