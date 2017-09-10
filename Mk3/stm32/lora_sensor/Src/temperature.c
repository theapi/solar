/*
 * temperature.c
 *
 */
#include "temperature.h"

/* Temperature sensor calibration value address */
#define TEMP130_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007A))
#define VDD_CALIB ((uint16_t) (300))
#define VDD_APPLI ((uint16_t) (330))

extern ADC_HandleTypeDef hadc;

/**
 * The temperature outside, divide by 10 to get degrees.
 *
 * This is horrible as TEMPERATURE_external() must be called first
 */
int16_t TEMPERATURE_external() {
    HAL_ADC_PollForConversion(&hadc, 100);
    uint32_t val = HAL_ADC_GetValue(&hadc);
    int16_t deg = -41;

    /*
The TMP36 is specified from −40°C to +125°C, provides a 750 mV output at 25°C.
The TMP36 has an output scale factor of 10 mV/°C.
4095 = 3260mV (measured on the 3.3V rail)
1bit = 3260 / 4095 = 0.796
     */
    uint32_t mv = val * 0.796F;
    if (mv > 500) {
        deg = (mv - 500);
    } else {
        deg = (500 - mv)* -1;
    }

    // To get actual degrees, use a float & divide by 10.
    return deg;
}

/**
 * The on chip temperature.
 *
 * This is horrible as TEMPERATURE_external() must be called first
 */
int16_t TEMPERATURE_cpu() {
    HAL_ADC_PollForConversion(&hadc, 100);
    uint32_t val = HAL_ADC_GetValue(&hadc);
    return ComputeTemperature(val);
}

/**
 * Datasheet example code A.8.17
 */
int32_t ComputeTemperature(uint32_t measure) {
    int32_t temperature;
    temperature = ((measure * VDD_APPLI / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
    temperature = temperature * (int32_t)(130 - 30);
    temperature = temperature / (int32_t)(*TEMP130_CAL_ADDR - *TEMP30_CAL_ADDR);
    temperature = temperature + 30;
    return(temperature);
}

