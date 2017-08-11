/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

#include "string.h"

#include "rfm95.h"
#include "payload_garden.h"
#include "ads1015.h"
#include "battery.h"
#include "temperature.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

volatile uint8_t dio0_action = 0;
MAIN_StateTypeDef state;

/* USER CODE END 0 */

int main(void) {

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC_Init();
    MX_I2C1_Init();
    MX_RTC_Init();
    MX_USART1_UART_Init();
    MX_SPI2_Init();

    /* USER CODE BEGIN 2 */

    /* Enable Ultra low power mode */
    HAL_PWREx_EnableUltraLowPower();
    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_HSI);

    /* Buffer used for transmission on USART1 */
    char tx1_buffer[120];

    uint8_t count = 0;

    /* SPI chip select high */
    HAL_GPIO_WritePin(GPIOB, SPI2_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(30);
    //RFM95_init(&hspi2);

    uint8_t payload_buff[14];
    PAYLOAD_Garden payload_garden;
    payload_garden.MessageType = 50;
    payload_garden.MessageId = 0;
    payload_garden.VCC = 0;
    payload_garden.ChargeMa = 0;

    // Start in sensing mode.
    state = MAIN_STATE_SENSE;

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        /* Do some work */
        if (state == MAIN_STATE_SENSE) {
            HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);

            payload_garden.MessageId = count;
            count++;

            payload_garden.VCC = BATTERY_vcc();
            payload_garden.ChargeMv = BATTERY_ChargeMv();
            payload_garden.ChargeMa = BATTERY_ChargeMa();
            payload_garden.Temperature = TEMPERATURE_external();
            payload_garden.CpuTemperature = TEMPERATURE_cpu();

            sprintf(tx1_buffer, "id:%d, vcc:%d, mv:%d, ma:%d, C:%d, cpuC:%d\n",
                    count,
                    payload_garden.VCC,
                    payload_garden.ChargeMv,
                    payload_garden.ChargeMa,
                    payload_garden.Temperature,
                    payload_garden.CpuTemperature);
            HAL_UART_Transmit(&huart1, (uint8_t*) tx1_buffer, strlen(tx1_buffer), 1000);

            PAYLOAD_Garden_serialize(payload_garden, payload_buff);
            //RFM95_send(&hspi2, payload_buff, 14);

            state = MAIN_STATE_TX;
        }

        /* Do nothing while the transmission is in progress */
        else if (state == MAIN_STATE_TX) {
            if (dio0_action == 1) {
                RFM95_setMode(&hspi2, RFM95_MODE_SLEEP);

                state = MAIN_STATE_SLEEP;
            }

            //TMP while RFM is diabled
            state = MAIN_STATE_SLEEP;
        }

        /* Now that all the work is done, sleep until its time to do it all again */
        else if (state == MAIN_STATE_SLEEP) {
            //TMP while RFM is diabled
            HAL_Delay(1000);

//            HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_RESET);
//
//            /* Turn off the pin interrupts */
//            HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
//
//            HAL_SuspendTick();
//
//            /* Enter Stop Mode */
//            HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2,
//            RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
//            HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
//            HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//            HAL_ResumeTick();
//
//            /* Turn on the pin interrupts */
//            HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

            state = MAIN_STATE_SENSE;
        }

    }
    /* USER CODE END 3 */

}

/** System Clock Configuration
 */
void SystemClock_Config(void) {

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure the main internal regulator output voltage 
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks 
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_RTC;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time 
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick 
     */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/**
 * Callback for RTC_IRQHandler()
 */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
    // Wake up
}

/**
 * Callback for HAL_GPIO_EXTI_IRQHandler() in EXTI4_15_IRQHandler().
 *
 * Handles the interrupts which occur on button press.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == DIO0_Pin) {
        dio0_action = 1;
    }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void _Error_Handler(char * file, int line) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    char buffer[80];
    sprintf(buffer, "Error: %d\n", line);
    HAL_UART_Transmit(&huart1, (uint8_t*) buffer, strlen(buffer), 5000);
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, LED_Pin);
        HAL_Delay(100);
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
