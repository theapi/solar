#include "ads1015.h"

HAL_StatusTypeDef ADS1015_writeRegister(I2C_HandleTypeDef* hi2c,
        uint8_t i2cAddress, uint8_t addr, uint16_t val) {
    //@todo implement ADS1015_writeRegister
    return HAL_OK;
}

uint16_t ADS1015_readRegister(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress,
        uint8_t addr) {
    //@todo implement ADS1015_readRegister
    return 0;
}

uint16_t ADS1015_SingleEnded(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress,
        uint8_t channel, Gain_TypeDef gain) {
    if (channel > 3) {
        return 0;
    }

    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
            ADS1015_REG_CONFIG_CLAT_NONLAT | // Non-latching (default val)
            ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1015_REG_CONFIG_CMODE_TRAD | // Traditional comparator (default val)
            ADS1015_REG_CONFIG_DR_1600SPS | // 1600 samples per second (default)
            ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= gain;

    // Set single-ended input channel
    switch (channel) {
    case (0):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
        break;
    case (1):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
        break;
    case (2):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
        break;
    case (3):
        config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
        break;
    }

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    ADS1015_writeRegister(hi2c, i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    HAL_Delay(ADS1015_CONVERSIONDELAY);

    // Read the conversion results
    // Shift 12-bit results right 4 bits for the ADS1015
    return ADS1015_readRegister(hi2c, i2cAddress, ADS1015_REG_POINTER_CONVERT) >> 4;
}
