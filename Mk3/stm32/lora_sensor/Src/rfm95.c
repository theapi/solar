#include "rfm95.h"

void RFM95_init(SPI_HandleTypeDef *hspi) {
    /* Allow module time to power on reset */
    HAL_Delay(30);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
  HAL_Delay(30);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI chip select high */
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(30);

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

    /* Set up FIFO
     * Configure the 256 byte FIFO for either receive or transmit,
     * but not both at the same time.
     */
    RFM95_writeRegister(hspi, RFM95_REG_FIFO_TX_BASE_ADDR, 0);
    RFM95_writeRegister(hspi, RFM95_REG_FIFO_RX_BASE_ADDR, 0);

    /* Set frequency */
    // {0x06D9, 0x0700, 0x0800}, //868MHz from HopeRf demo code
    RFM95_writeRegister(hspi, RFM95_REG_FRF_MSB, 0xD9);
    RFM95_writeRegister(hspi, RFM95_REG_FRF_MID, 0x00);
    RFM95_writeRegister(hspi, RFM95_REG_FRF_LSB, 0x00);

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

HAL_StatusTypeDef RFM95_writeRegisterBurst(SPI_HandleTypeDef* hspi, uint8_t addr, uint8_t *data, uint8_t len) {
    //@todo burst write.
    uint8_t cmd[1];
    /* Enable the write bit to the address. */
    cmd[0] = addr | RFM95_WRITE_MASK;
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit (hspi, cmd, 1, 100);
    /* Send the data without ending the frame; BURST mode. */
    HAL_SPI_Transmit (hspi, data, len, 100);
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);

    return HAL_OK;
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
    /* Ensure LoRa mode is set in RFM95_REG_OP_MODE. */
    return RFM95_writeRegister(hspi, RFM95_REG_OP_MODE, mode | RFM95_LONG_RANGE_MODE);
}

HAL_StatusTypeDef RFM95_send(SPI_HandleTypeDef* hspi, uint8_t* data, uint8_t len) {
    RFM95_setMode(hspi, RFM95_MODE_STDBY);

    // Position at the beginning of the FIFO
    RFM95_writeRegister(hspi, RFM95_REG_FIFO_ADDR_PTR, 0);

    // Packet format is preamble + explicit-header + payload + crc
    // Explicit Header Mode
    // payload is TO + FROM + ID + FLAGS + message data
    // Same as Radiohead library.

    // The headers
    uint8_t txHeaderTo = RFM95_BROADCAST_ADDRESS;
    uint8_t txHeaderFrom = RFM95_BROADCAST_ADDRESS;
    uint8_t txHeaderId = 0;
    uint8_t txHeaderFlags = 0;
    RFM95_writeRegister(hspi, RFM95_REG_FIFO, txHeaderTo);
    RFM95_writeRegister(hspi, RFM95_REG_FIFO, txHeaderFrom);
    RFM95_writeRegister(hspi, RFM95_REG_FIFO, txHeaderId);
    RFM95_writeRegister(hspi, RFM95_REG_FIFO, txHeaderFlags);
    // The message data
    RFM95_writeRegisterBurst(hspi, RFM95_REG_FIFO, data, len);
    RFM95_writeRegister(hspi, RFM95_REG_PAYLOAD_LENGTH, len + RFM95_HEADER_LEN);

    // Interrupt on TxDone
    RFM95_writeRegister(hspi, RFM95_REG_DIO_MAPPING1, 0x40);
    // Start the transmitter
    RFM95_setMode(hspi, RFM95_MODE_TX);

    return HAL_OK;
}
