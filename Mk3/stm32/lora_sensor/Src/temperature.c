/*
 * temperature.c
 *
 */
#include "temperature.h"

extern ADC_HandleTypeDef hadc;

/**
 * The temperature outside.
 */
int16_t TEMPERATURE_external() {
    //@todo reconfigure to only one adc channel
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 100);
    uint32_t val = HAL_ADC_GetValue(&hadc);

    HAL_ADC_Stop(&hadc);
    return val;
}

/**
 * The on chip temperature.
 */
int16_t TEMPERATURE_cpu() {
    //@todo implement TEMPERATURE_cpu()
    return 35;
}
