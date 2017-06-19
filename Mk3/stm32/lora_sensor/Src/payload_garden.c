#include "payload_garden.h"


void PAYLOAD_Garden_serialize(PAYLOAD_Garden payload, uint8_t buffer[PAYLOAD_Garden_SIZE]) {
    buffer[0] = payload.MessageType;
    buffer[1] = payload.MessageId;

    buffer[2] = (payload.VCC >> 8);
    buffer[3] = payload.VCC;
    buffer[4] = (payload.ChargeMv >> 8);
    buffer[5] = payload.ChargeMv;
    buffer[6] = (payload.ChargeMa >> 8);
    buffer[7] = payload.ChargeMa;
    buffer[8] = (payload.Light >> 8);
    buffer[9] = payload.Light;
    buffer[10] = (payload.CpuTemperature >> 8);
    buffer[11] = payload.CpuTemperature;
    buffer[12] = (payload.Temperature >> 8);
    buffer[13] = payload.Temperature;
}
