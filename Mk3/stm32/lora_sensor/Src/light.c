/*
 * light.c
 */

#include "light.h"
#include "bh1750.h"

extern I2C_HandleTypeDef hi2c1;

uint16_t LIGHT_lux() {
    return BH1750_lux(&hi2c1, BH1750_ADDRESS);
}

/**
 * Power down the sensor.
 */
void LIGHT_powerDown() {
    BH1750_powerDown(&hi2c1, BH1750_ADDRESS);
}

/**
 * Power up the sensor.
 */
void LIGHT_powerOn() {
    BH1750_powerOn(&hi2c1, BH1750_ADDRESS);
}
