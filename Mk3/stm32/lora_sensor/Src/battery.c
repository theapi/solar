/**
 * battery.c
 */

#include "ads1015.h"
#include "battery.h"

extern I2C_HandleTypeDef hi2c1;

uint16_t BATTERY_vcc() {
    /*
     * 2x gain   +/- 2.048V  1 bit = 1mV
     * 1x gain   +/- 4.096V  1 bit = 2mv
     * Battery is measured via a voltage divided, which halves the voltage.
     * 3 X rechargable batteries, allowing for 1.5V per battery, 4.5V
     * 4.5V on the voltage divider = 2.25V
     */
    uint16_t val = ADS1015_SingleEnded(&hi2c1, ADS1015_ADDRESS, 3, ADS1015_GAIN_ONE);
    /*
     * x2 for the 2mv per bit
     * x2 for the voltage divider
     */
    return val * 4;
}

uint16_t BATTERY_ChargeMv() {
    /*
     * Voltage divider = 300 / 100
     */
    uint16_t val = ADS1015_SingleEnded(&hi2c1, ADS1015_ADDRESS, 2, ADS1015_GAIN_TWO);
    return val * 4;
}

int16_t BATTERY_ChargeMa() {
    int16_t val = ADS1015_Differential_0_1(&hi2c1, ADS1015_ADDRESS, ADS1015_GAIN_TWO);
    /*
     * Invert as its a negative value referenced to ground when charging.
     */
    return val *-1;
}
