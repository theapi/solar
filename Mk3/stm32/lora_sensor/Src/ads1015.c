#include "ads1015.h"

HAL_StatusTypeDef ADS1015_writeRegister(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress, uint8_t addr, uint16_t val) {
    uint8_t data[3];

    data[0] = addr;
    data[1] = val >> 8;    // MSB byte of 16bit data
    data[2] = val;       // LSB byte of 16bit data
    HAL_I2C_Master_Transmit(hi2c, i2cAddress, data, 3, 100);

    return HAL_OK;
}

uint16_t ADS1015_readRegister(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress, uint8_t addr) {
    uint8_t reg_ptr = addr;
    uint8_t buffer[2];

    /* first set the register pointer to the register wanted to be read */
    HAL_I2C_Master_Transmit(hi2c, i2cAddress, &reg_ptr, 1, 100);  // note the & operator which gives us the address of the register_pointer variable

    /* receive the 2 x 8bit data into the receive buffer */
    HAL_I2C_Master_Receive(hi2c, i2cAddress, buffer, 2, 100);

    /* Return as uint16_t */
    return (buffer[0] << 8) | buffer[1];
}

uint16_t ADS1015_SingleEnded(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress, uint8_t channel, Gain_TypeDef gain) {
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

int16_t ADS1015_Differential_0_1(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress, Gain_TypeDef gain) {
    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
            ADS1015_REG_CONFIG_CLAT_NONLAT | // Non-latching (default val)
            ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1015_REG_CONFIG_CMODE_TRAD | // Traditional comparator (default val)
            ADS1015_REG_CONFIG_DR_1600SPS | // 1600 samples per second (default)
            ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= gain;

    // Set channels
    config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    ADS1015_writeRegister(hi2c, i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    HAL_Delay(ADS1015_CONVERSIONDELAY);

    // Read the conversion results
    uint16_t res = ADS1015_readRegister(hi2c, i2cAddress,
    ADS1015_REG_POINTER_CONVERT) >> 4;

    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF) {
        // negative number - extend the sign to 16th bit
        res |= 0xF000;
    }
    return (int16_t) res;

}

int16_t ADS1015_Differential_2_3(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress, Gain_TypeDef gain) {
    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
            ADS1015_REG_CONFIG_CLAT_NONLAT | // Non-latching (default val)
            ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1015_REG_CONFIG_CMODE_TRAD | // Traditional comparator (default val)
            ADS1015_REG_CONFIG_DR_1600SPS | // 1600 samples per second (default)
            ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= gain;

    // Set channels
    config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3;          // AIN2 = P, AIN3 = N

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    ADS1015_writeRegister(hi2c, i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    HAL_Delay(ADS1015_CONVERSIONDELAY);

    // Read the conversion results
    uint16_t res = ADS1015_readRegister(hi2c, i2cAddress,
    ADS1015_REG_POINTER_CONVERT) >> 4;

    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF) {
        // negative number - extend the sign to 16th bit
        res |= 0xF000;
    }
    return (int16_t) res;

}

