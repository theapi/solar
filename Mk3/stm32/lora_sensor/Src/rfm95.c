#include "rfm95.h"

void RFM95_init(SPI_HandleTypeDef *hspi) {
    if (RFM95_writeRegister(hspi, RFM95_REG_OP_MODE, RFM95_MODE_SLEEP)  != HAL_OK) {
        Error_Handler();
    }

    /* Wait a bit for sleep mode to be activated */
    HAL_Delay(10);

    /* Now LoRa mode can be set */
    if (RFM95_writeRegister(hspi, RFM95_REG_OP_MODE, RFM95_LONG_RANGE_MODE)  != HAL_OK) {
        Error_Handler();
    }

    /* Check the register was set. */
    uint8_t val = RFM95_readRegister(hspi, RFM95_REG_OP_MODE);
    if (val != (RFM95_MODE_SLEEP | RFM95_LONG_RANGE_MODE)) {
        // Failed to set the register, can't continue.
        Error_Handler();
    }

    /* Set config */
//  1d,     1e,      26
//    { 0x72,   0x74,    0x00}, // Bw125Cr45Sf128 (the chip default)
//    { 0x92,   0x74,    0x00}, // Bw500Cr45Sf128
//    { 0x48,   0x94,    0x00}, // Bw31_25Cr48Sf512
//    { 0x78,   0xc4,    0x00}, // Bw125Cr48Sf4096

    // Bw500Cr45Sf128
    RFM95_writeRegister(hspi, RFM95_REG_MODEM_CONFIG1, 0x92);
    RFM95_writeRegister(hspi, RFM95_REG_MODEM_CONFIG2, 0x74);
    RFM95_writeRegister(hspi, RFM95_REG_MODEM_CONFIG3, 0x00);

    /* Set TX power with PA_BOOST */
    // 0x09F6,                   //11dbm  from HopeRf demo code
    RFM95_writeRegister(hspi, RFM95_REG_PA_CONFIG, RFM_PA_SELECT | 0xF6);

}

HAL_StatusTypeDef RFM95_writeRegister(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t val) {
    uint8_t data[2];
    /* Enable the write bit to the address. */
    data[0] = addr | RFM95_WRITE_MASK;
    data[1] = val;
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit (hspi, data, 2, 100);
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);

    return status;
}

uint8_t RFM95_readRegister(SPI_HandleTypeDef *hspi, uint8_t addr) {
    uint8_t cmd[2] = {0, 0};
    uint8_t data[2] = {0, 0};

    /* Add the read command to the address. */
    cmd[0] = addr & ~RFM95_WRITE_MASK;

    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
    /* Transmit 1 byte and receive 1. Actually receive 2, but the first doesn't matter. */
    HAL_SPI_TransmitReceive(hspi, cmd, data, 2, 100);
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);

    /* The returned value is the second byte */
    return data[1];
}

HAL_StatusTypeDef RFM95_setMode(SPI_HandleTypeDef *hspi, uint8_t mode) {
    // Always ensure LoRa mode is set in RFM95_REG_OP_MODE.
    return RFM95_writeRegister(hspi, RFM95_REG_OP_MODE, mode);
}

