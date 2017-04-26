
/**
 * Speck-256-ECB
 */

#include <Crypto.h>
#include <SpeckSmall.h>
#include "Payload.h"
#include "GardenPayload.h"

uint8_t msg_id = 0;
theapi::GardenPayload payload = theapi::GardenPayload();

byte key[32] = {0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18,
              0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10,
              0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
              0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};

SpeckSmall cipher;
byte encrypted_buffer[theapi::GardenPayload::SIZE];

void setup() {
  Serial.begin(115200);
  Serial.println();

  cipher.setKey(key, 32);

  payload.setMsgId(msg_id);
  payload.setVcc(1234);
  payload.setChargeMv(5678);
  payload.setChargeMa(72);
  payload.setLight(69);
  payload.setSoil(76);
  payload.setTemperature(79);

}

void loop() {

  payload.setMsgId(++msg_id);

  uint8_t payload_buf[payload.size()];
  payload.serialize(payload_buf);
 
  // Encrypt
  cipher.encryptBlock(encrypted_buffer, payload_buf);

  // Decrypt
  uint8_t decrypted_buf[payload.size()];
  cipher.decryptBlock(decrypted_buf, encrypted_buffer);
  payload.unserialize(decrypted_buf);

  Serial.print("Decrypted: ");
  Serial.print(payload.getMsgType()); Serial.print(", ");
  Serial.print(payload.getMsgId()); Serial.print(", ");
  Serial.print(payload.getVcc()); Serial.print(", ");
  Serial.print(payload.getChargeMv()); Serial.print(", ");
  Serial.print(payload.getChargeMa()); Serial.print(", ");
  Serial.print(payload.getLight()); Serial.print(", ");
  Serial.print(payload.getSoil()); Serial.print(", ");
  Serial.println(payload.getTemperature());
  Serial.println();

  
  delay(2000);
  
}
