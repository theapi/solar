#include "GardenPayload.h"
#include "Payload.h"

//using namespace theapi;

namespace theapi {

  GardenPayload::GardenPayload() {
    setMsgType();
    _payload.msg_id = 0;
    _payload.a = 0;
    _payload.b = 0;
    _payload.c = 0;
    _payload.d = 0;
    _payload.e = 0;
    _payload.f = 0;
  }

  uint8_t GardenPayload::size() {
    return SIZE;
  }

  void GardenPayload::setMsgType() {
    _payload.msg_type = theapi::Payload::GARDEN;
  }

  uint8_t GardenPayload::getMsgType() {
    return _payload.msg_type;
  }

  // The id, not neccessarily unique, of the message.
  uint8_t GardenPayload::getMsgId()
  {
    return _payload.msg_id;
  }

  void GardenPayload::setMsgId(uint8_t id)
  {
    _payload.msg_id = id;
  }

  // The first integer data
  int16_t GardenPayload::getVcc()
  {
    return _payload.a;
  }

  void GardenPayload::setVcc(int16_t val)
  {
    _payload.a = val;
  }

  // The second integer data
  int16_t GardenPayload::getChargeMv()
  {
    return _payload.b;
  }

  void GardenPayload::setChargeMv(int16_t val)
  {
    _payload.b = val;
  }

  // The third integer data
  int16_t GardenPayload::getChargeMa()
  {
    return _payload.c;
  }

  void GardenPayload::setChargeMa(int16_t val)
  {
    _payload.c = val;
  }

  // The forth integer data
  int16_t GardenPayload::getLight()
  {
    return _payload.d;
  }

  void GardenPayload::setLight(int16_t val)
  {
    _payload.d = val;
  }

  int16_t GardenPayload::getSoil()
  {
    return _payload.e;
  }

  void GardenPayload::setSoil(int16_t val)
  {
    _payload.e = val;
  }

  int16_t GardenPayload::getTemperature()
  {
    return _payload.f;
  }

  void GardenPayload::setTemperature(int16_t val)
  {
    _payload.f = val;
  }

  // Populates the given array with the payload data
  void GardenPayload::serialize(uint8_t payload[GardenPayload::SIZE])
  {
    payload[0] = _payload.msg_type;
    payload[1] = _payload.msg_id;

    payload[2] = (_payload.a >> 8);
    payload[3] = _payload.a;
    payload[4] = (_payload.b >> 8);
    payload[5] = _payload.b;
    payload[6] = (_payload.c >> 8);
    payload[7] = _payload.c;
    payload[8] = (_payload.d >> 8);
    payload[9] = _payload.d;
    payload[10] = (_payload.e >> 8);
    payload[11] = _payload.e;
    payload[12] = (_payload.f >> 8);
    payload[13] = _payload.f;

  /*
    payload[16] = (_payload.timestamp >> 24);
    payload[17] = (_payload.timestamp >> 16);
    payload[18] = (_payload.timestamp >> 8);
    payload[19] = _payload.timestamp;
  */
  }

  // Parse the read byte data
  void GardenPayload::unserialize(uint8_t payload[GardenPayload::SIZE])
  {
    _payload.msg_type = payload[0];
    _payload.msg_id = payload[1];

    //_payload.msg_id = (payload[2] << 8) | payload[3];
    //_payload.vcc = (payload[4] << 8) | payload[5];

    _payload.a = (payload[2] << 8) | payload[3];
    _payload.b = (payload[4] << 8) | payload[5];

    _payload.c = (payload[6] << 8) | payload[7];
    _payload.d = (payload[8] << 8) | payload[9];

    _payload.e = (payload[10] << 8) | payload[11];
    _payload.f = (payload[12] << 8) | payload[13];

    // Need to cast to unsigned long for the larger bit shift
    //_payload.timestamp = ((uint32_t)payload[16] << 24) | ((uint32_t)payload[17] << 16) | ((uint32_t)payload[18] << 8) | (uint32_t)payload[19];

  }

}
