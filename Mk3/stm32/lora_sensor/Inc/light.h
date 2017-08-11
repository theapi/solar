/*
 * light.h
 *
 */

#ifndef __light_H
#define __light_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l0xx_hal.h"
#include "main.h"
#include "stdint.h"

/**
 * The ambiant light measurement.
 */
uint16_t LIGHT_lux();


#ifdef __cplusplus
}
#endif
#endif /* __light_H */
