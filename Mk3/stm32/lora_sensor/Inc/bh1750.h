/*
 * bh1750.h
 *
 */

#ifndef __bh1750_H
#define __bh1750_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "main.h"
#include "stdint.h"

/*
 * I2C ADDRESS/BITS
 */
#define BH1750_ADDRESS  (0x23 << 1) // 7 bit address. ADDR pin to GND

/*
 * No active state.
 */
#define BH1750_INS_POWER_DOWN 0x00

/*
 * Waiting for measurement command.
 */
#define BH1750_INS_POWER_ON 0x01

/*
 * Reset Data register value.
 * Reset command is not acceptable in Power Down mode.
 */
#define BH1750_INS_RESET 0x07

/* Start measurement at 1lx resolution.
 * Measurement time is approx 120ms.
 */
#define BH1750_MODE_CONTINUOUS_HIGH_RES  0x10

/* Start measurement at 0.5lx resolution.
 * Measurement Time is typically 120ms.
 */
#define BH1750_MODE_CONTINUOUS_HIGH_RES_2 0x11

/* Start measurement at 4lx resolution.
 * Measurement time is approx 16ms.
 */
#define BH1750_MODE_CONTINUOUS_LOW_RES  0x13

/* Start measurement at 1lx resolution.
 * Measurement Time is typically 120ms.
 * It is automatically set to Power Down mode after measurement.
 */
#define BH1750_MODE_ONE_TIME_HIGH_RES  0x20

/*
 * Start measurement at 0.5lx resolution.
 * Measurement Time is typically 120ms.
 * It is automatically set to Power Down mode after measurement.
 */
#define BH1750_MODE_ONE_TIME_HIGH_RES_2  0x21

/*
 * Start measurement at 4lx resolution.
 * Measurement Time is typically 16ms.
 * It is automatically set to Power Down mode after measurement.
 */
#define BH1750_MODE_ONE_TIME_LOW_RES  0x23

void BH1750_powerDown(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress);
void BH1750_powerOn(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress);
uint16_t BH1750_lux(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress);

#ifdef __cplusplus
}
#endif
#endif /* __bh1750_H */
