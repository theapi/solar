
/**
 * AES-128-ECB
 * 
 * https://rweather.github.io/arduinolibs/crypto.html
 * https://github.com/rweather/arduinolibs/tree/master/libraries/Crypto
 */

#include <Crypto.h>
#include <AES.h>
#include "Payload.h"
#include "GardenPayload.h"

#define ENCRYPTION_BUFFER_SIZE 32

uint8_t msg_id = 0;
theapi::GardenPayload payload = theapi::GardenPayload();


byte key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

AES128 cipher;
uint8_t encrypted_buffer[ENCRYPTION_BUFFER_SIZE];
uint8_t decrypted_buf[ENCRYPTION_BUFFER_SIZE];

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setup");

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
  Serial.print("Encrypting msg_id: ");
  Serial.println(msg_id);

  uint8_t payload_buf[payload.size()];
  payload.serialize(payload_buf);

  // Encrypt
  cipher.encryptBlock(encrypted_buffer, payload_buf);

  // Decrypt
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
