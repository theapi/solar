/**
 * battery.c
 */

#include "ads1015.h"
#include "battery.h"

extern I2C_HandleTypeDef hi2c1;

uint16_t BATTERY_vcc() {
    return ADS1015_SingleEnded(&hi2c1, ADS1015_ADDRESS, 3, ADS1015_GAIN_TWO);
}

uint16_t BATTERY_ChargeMv() {
    return ADS1015_SingleEnded(&hi2c1, ADS1015_ADDRESS, 2, ADS1015_GAIN_TWO);
}

int16_t BATTERY_ChargeMa() {
    return ADS1015_Differential_0_1(&hi2c1, ADS1015_ADDRESS, ADS1015_GAIN_TWO);
}
