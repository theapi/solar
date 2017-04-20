#include "TheapiPayloadPing.h"
#include "TheapiPayload.h"

TheapiPayloadPing::TheapiPayloadPing() {
  setMsgType();
  _payload.msg_id = 0;
}

uint8_t TheapiPayloadPing::getPayloadSize() {
  return TheapiPayload_SIZE;
}

void TheapiPayloadPing::setMsgType() {
  _payload.msg_type = MSG_TYPE_PING;
}

uint8_t TheapiPayloadPing::getMsgType() {
  return _payload.msg_type;
}

// The id, not neccessarily unique, of the message.
uint8_t TheapiPayloadPing::getMsgId() {
  return _payload.msg_id;
}

void TheapiPayloadPing::setMsgId(uint8_t id) {
  _payload.msg_id = id;
}

// Populates the given array with the payload data
void TheapiPayloadPing::serialize(uint8_t payload[TheapiPayloadPing_SIZE]) {
  payload[0] = _payload.msg_type;
  payload[1] = _payload.msg_id;
}

// Parse the read byte data
void TheapiPayloadPing::unserialize(uint8_t payload[TheapiPayloadPing_SIZE]) {
  _payload.msg_type = payload[0];
  _payload.msg_id = payload[1];
}
