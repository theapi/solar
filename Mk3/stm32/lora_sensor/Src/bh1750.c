
#include "bh1750.h"


HAL_StatusTypeDef BH1750_setRegister(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress, uint8_t addr) {
    uint8_t data[1];

    data[0] = addr;
    HAL_I2C_Master_Transmit(hi2c, i2cAddress, data, 1, 100);

    return HAL_OK;
}

void BH1750_powerDown(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress) {
    BH1750_setRegister(hi2c, i2cAddress, BH1750_INS_POWER_DOWN);
}

void BH1750_powerOn(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress) {
    BH1750_setRegister(hi2c, i2cAddress, BH1750_INS_POWER_ON);
}

uint16_t BH1750_lux(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress) {
    uint8_t buffer[2];

    /* Take one high res reading then power down. */
    BH1750_setRegister(hi2c, i2cAddress, BH1750_MODE_ONE_TIME_HIGH_RES);
    /* Measurement Time is typically 120ms. */
    HAL_Delay(160);

    /* Receive the 2 x 8bit data into the receive buffer */
    HAL_I2C_Master_Receive(hi2c, i2cAddress, buffer, 2, 100);

    /* Return as uint16_t */
    return ((buffer[0] << 8) | buffer[1]) / 1.2;
}
