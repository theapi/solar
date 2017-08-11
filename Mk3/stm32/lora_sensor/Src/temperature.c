/*
 * temperature.c
 *
 */
#include "temperature.h"

//extern ADC_HandleTypeDef hadc;

/**
 * The temperature outside.
 */
int16_t TEMPERATURE_external() {
    //@todo implement TEMPERATURE_external()
    return 25;
}

/**
 * The on chip temperature.
 */
int16_t TEMPERATURE_cpu() {
    //@todo implement TEMPERATURE_cpu()
    return 35;
}
