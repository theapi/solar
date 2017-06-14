#include "rfm95.h"

void RFM95_init(SPI_HandleTypeDef *hspi) {
    if (RFM95_writeRegister(hspi, RFM95_REG_OP_MODE, RFM95_MODE_SLEEP | RFM95_LONG_RANGE_MODE)  != HAL_OK) {
        Error_Handler();
    }
//    if (RFM95_writeRegister(hspi, RFM95_REG_OP_MODE, RFM95_LONG_RANGE_MODE)  != HAL_OK) {
//        Error_Handler();
//    }

    /* Give it time to sleep */
    HAL_Delay(30);

    /* Check the register was set. */
    uint8_t val = RFM95_readRegister(hspi, RFM95_REG_OP_MODE);
    if (val != RFM95_MODE_SLEEP | RFM95_LONG_RANGE_MODE) {
        // Failed to set the register, can't continue.
        Error_Handler();
    }

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

//    uint8_t val[1];
//    val[0] = 0;
//    //val[0] = addr & ~RFM95_WRITE_MASK;
//    //val[1] = 0;
//
//    // First send the read command.
//    uint8_t data[1];
//    // Disable the write bit to the address
//    data[0] = addr & ~RFM95_WRITE_MASK;
//    data[1] = 0;
//    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
//    if (HAL_SPI_Transmit (hspi, data, 2, 100) != HAL_OK) {
//        Error_Handler();
//    }
//    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
//    if (HAL_SPI_Receive (hspi, data, 2, 100) != HAL_OK) {
//        Error_Handler();
//    }
//    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);
//
//    return data[1];
}
