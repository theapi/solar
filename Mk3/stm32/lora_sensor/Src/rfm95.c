#include "rfm95.h"

void RFM95_init(SPI_HandleTypeDef* spiHandle) {

}

HAL_StatusTypeDef RFM95_writeRegister(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t val) {
    uint8_t data[2];
    // Enable the write bit to the address
    data[0] = addr | RFM95_WRITE_MASK;
    data[1] = val;
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit (hspi, data, 2, 100);
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);
    return status;
}

uint8_t RFM95_readRegister(SPI_HandleTypeDef *hspi, uint8_t addr) {
    // First send the read command.
    uint8_t data[1];
    // Disable the write bit to the address
    data[0] = addr & ~RFM95_WRITE_MASK;
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit (hspi, data, 1, 100) != HAL_OK) {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);

    // Now get the value requested.
    uint8_t val[1];
    val[0] = 0;
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Receive (hspi, val, 1, 100) != HAL_OK) {
        Error_Handler();
    }
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);

    return val[0];
}
