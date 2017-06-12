#include "rfm95.h"

void RFM95_init(SPI_HandleTypeDef* spiHandle) {

}

HAL_StatusTypeDef RFM95_writeRegister(SPI_HandleTypeDef *hspi, uint8_t addr, uint8_t val) {
    uint8_t data[2];
    // Enable the write bit to the address
    data[0] = addr | RFM95_WRITE_MASK;
    data[1] = val;
    HAL_StatusTypeDef status = HAL_SPI_Transmit (hspi, data, 2, 100);
    return status;
}

uint8_t RFM95_readRegister(SPI_HandleTypeDef *hspi, uint8_t addr) {
    // First send the read command.
    uint8_t data[1];
    // Disable the write bit to the address
    data[0] = addr & ~RFM95_WRITE_MASK;
    switch (HAL_SPI_Transmit (hspi, data, 1, 100)) {
      case HAL_TIMEOUT:
      case HAL_ERROR:
        Error_Handler();
        break;
      default:
        break;
    }

    // Now get the value requested.
    uint8_t val[1];
    val[0] = 0;
    switch (HAL_SPI_Receive (hspi, val, 1, 100)) {
      case HAL_TIMEOUT:
      case HAL_ERROR:
        Error_Handler();
        break;
      default:
        break;
    }

    return val[0];
}
