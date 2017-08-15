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
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 100);
    uint32_t val = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);

    // 4095 = 3260mV (measured on the 3.3V rail)
    // 1bit = 3260 / 4095 = 0.796
    return val * 0.796F;
}

/**
 * The on chip temperature.
 */
int16_t TEMPERATURE_cpu() {
    //@todo implement TEMPERATURE_cpu()
    return 35;
}
