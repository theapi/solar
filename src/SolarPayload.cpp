#include "SolarPayload.h"
#include "Payload.h"

//using namespace theapi;

namespace theapi {

  SolarPayload::SolarPayload() {
    setMsgType();
    _payload.DeviceId = 0;
    _payload.MessageId = 0;
    _payload.Flags = 0;
    _payload.VCC = 0;
    _payload.ChargeMv = 0;
    _payload.ChargeMa = 0;
    _payload.Light = 0;
    _payload.CpuTemperature = 0;
    _payload.Temperature = 0;
    _payload.rssi = 0;
    _payload.snr = 0;
    _payload.frq_error = 0;
  }

  uint8_t SolarPayload::size() {
    return SIZE;
  }

  uint8_t SolarPayload::dataSize() {
    return DATA_SIZE;
  }

  void SolarPayload::setMsgType() {
    _payload.MessageType = theapi::Payload::SOLAR;
  }

  uint8_t SolarPayload::getMsgType() {
    return _payload.MessageType;
  }

  uint16_t SolarPayload::getDeviceId() {
    return _payload.DeviceId;
  }

  void SolarPayload::setDeviceId(uint16_t val) {
    _payload.DeviceId = val;
  }

  // The id, not neccessarily unique, of the message.
  uint8_t SolarPayload::getMsgId() {
    return _payload.MessageId;
  }

  void SolarPayload::setMsgId(uint8_t id) {
    _payload.MessageId = id;
  }

  uint8_t SolarPayload::getFlags() {
    return _payload.Flags;
  }

  void SolarPayload::setFlags(uint8_t byte) {
    _payload.Flags = byte;
  }

  uint16_t SolarPayload::getVcc() {
    return _payload.VCC;
  }

  void SolarPayload::setVcc(uint16_t val) {
    _payload.VCC = val;
  }

  uint16_t SolarPayload::getChargeMv() {
    return _payload.ChargeMv;
  }

  void SolarPayload::setChargeMv(uint16_t val) {
    _payload.ChargeMv = val;
  }

  int16_t SolarPayload::getChargeMa() {
    return _payload.ChargeMa;
  }

  void SolarPayload::setChargeMa(int16_t val) {
    _payload.ChargeMa = val;
  }

  uint16_t SolarPayload::getLight() {
    return _payload.Light;
  }

  void SolarPayload::setLight(uint16_t val) {
    _payload.Light = val;
  }

  int16_t SolarPayload::getCpuTemperature() {
    return _payload.CpuTemperature;
  }

  void SolarPayload::setCpuTemperature(int16_t val) {
    _payload.CpuTemperature = val;
  }

  int16_t SolarPayload::getTemperature() {
    return _payload.Temperature;
  }

  void SolarPayload::setTemperature(int16_t val) {
    _payload.Temperature = val;
  }

  int16_t SolarPayload::getRssi() {
    return _payload.rssi;
  }

  void SolarPayload::setRssi(int16_t val) {
    _payload.rssi = val;
  }

  int16_t SolarPayload::getSnr() {
    return _payload.snr;
  }

  void SolarPayload::setSnr(int16_t val) {
    _payload.snr = val;
  }

  int16_t SolarPayload::getFreqError() {
    return _payload.frq_error;
  }

  void SolarPayload::setFreqError(int16_t val) {
    _payload.frq_error = val;
  }

  // Populates the given buffer with the payload data.
  void SolarPayload::serialize(uint8_t buffer[SolarPayload::SIZE]) {
    buffer[0] = _payload.MessageType;
    buffer[1] = _payload.DeviceId;
    buffer[2] = _payload.MessageId;
    buffer[3] = _payload.Flags;
    buffer[4] = (_payload.VCC >> 8);
    buffer[5] = _payload.VCC;
    buffer[6] = (_payload.ChargeMv >> 8);
    buffer[7] = _payload.ChargeMv;
    buffer[8] = (_payload.ChargeMa >> 8);
    buffer[9] = _payload.ChargeMa;
    buffer[10] = (_payload.Light >> 8);
    buffer[11] = _payload.Light;
    buffer[12] = (_payload.CpuTemperature >> 8);
    buffer[13] = _payload.CpuTemperature;
    buffer[14] = (_payload.Temperature >> 8);
    buffer[15] = _payload.Temperature;
    buffer[16] = (_payload.rssi >> 8);
    buffer[17] = _payload.rssi;
    buffer[18] = (_payload.snr >> 8);
    buffer[19] = _payload.snr;
    buffer[20] = (_payload.frq_error >> 8);
    buffer[21] = _payload.frq_error;
  }

  // Parse the byte data from the buffer.
  void SolarPayload::unserialize(uint8_t buffer[SolarPayload::SIZE]) {
    _payload.MessageType = buffer[0];
    _payload.DeviceId = buffer[1];
    _payload.MessageId = buffer[2];
    _payload.Flags = buffer[3];
    _payload.VCC = (buffer[4] << 8) | buffer[5];
    _payload.ChargeMv = (buffer[6] << 8) | buffer[7];
    _payload.ChargeMa = (buffer[8] << 8) | buffer[9];
    _payload.Light = (buffer[10] << 8) | buffer[11];
    _payload.CpuTemperature = (buffer[12] << 8) | buffer[13];
    _payload.Temperature = (buffer[14] << 8) | buffer[15];
    _payload.rssi = (buffer[16] << 8) | buffer[17];
    _payload.snr = (buffer[18] << 8) | buffer[19];
    _payload.frq_error = (buffer[20] << 8) | buffer[21];
  }

}
