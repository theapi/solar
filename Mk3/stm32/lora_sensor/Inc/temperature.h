/*
 * temperature.h
 *
 */

#ifndef __temperature_H
#define __temperature_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "main.h"
#include "stdint.h"

/**
 * The temperature outside.
 */
int16_t TEMPERATURE_external();

/**
 * The on chip temperature.
 */
int16_t TEMPERATURE_cpu();

int32_t ComputeTemperature(uint32_t measure);

#ifdef __cplusplus
}
#endif
#endif /* __temperature_H */
