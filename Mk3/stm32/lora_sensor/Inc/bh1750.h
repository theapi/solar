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

/* I2C ADDRESS/BITS */
#define BH1750_ADDRESS  (0x23 << 1) // 7 bit address. ADDR pin to GND



#ifdef __cplusplus
}
#endif
#endif /* __bh1750_H */
