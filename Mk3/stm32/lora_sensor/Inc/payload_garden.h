
#ifndef __payload_garden_H
#define __payload_garden_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "main.h"
#include "stdint.h"

#define PAYLOAD_Garden_SIZE 14

typedef struct {
  uint8_t MessageType;
  uint8_t MessageId;
  uint16_t VCC;
  uint16_t ChargeMv;
  int16_t ChargeMa;
  uint16_t Light;
  int16_t CpuTemperature;
  int16_t Temperature;
} PAYLOAD_Garden;


// Creates a byte array for sending via the radio
void PAYLOAD_Garden_serialize(PAYLOAD_Garden payload, uint8_t buffer[PAYLOAD_Garden_SIZE]);

// Parse the read byte data from the radio
void PAYLOAD_Garden_unserialize(PAYLOAD_Garden payload, uint8_t buffer[PAYLOAD_Garden_SIZE]);

#ifdef __cplusplus
}
#endif
#endif /*__payload_garden_H */
