/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f411xe.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

#include <stdint.h>

// LED-Pins
#define GRUEN   (1U << 12)
#define ORANGE  (1U << 13)
#define ROT     (1U << 14)

#define ALLE_LEDS (GRUEN | ORANGE | ROT)

// GPIO initialisieren
void gpio_init(void)
{
    // Takt für Port D einschalten
    RCC->AHB1ENR |= (1U << 3);
    (void)RCC->AHB1ENR;

    // PD12, PD13, PD14: Mode-Bits löschen
    GPIOD->MODER &= ~((3U << 24) |
                      (3U << 26) |
                      (3U << 28));

    // Als Ausgang konfigurieren (01)
    GPIOD->MODER |= (1U << 24) |
                    (1U << 26) |
                    (1U << 28);

    // Push-Pull und keine Pull-Widerstände
    GPIOD->OTYPER &= ~ALLE_LEDS;
    GPIOD->PUPDR &= ~((3U << 24) |
                      (3U << 26) |
                      (3U << 28));

    // LEDs ausschalten
    GPIOD->ODR &= ~ALLE_LEDS;
}

// SysTick für 1 ms bei 16 MHz einrichten
void systick_init(void)
{
    SysTick->LOAD = 15999;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x5;
}

// Eigene Delay-Funktion
void delay_ms(uint32_t n)
{
    for (uint32_t i = 0; i < n; i++)
    {
        while ((SysTick->CTRL & (1U << 16)) == 0)
        {
        }
    }
}

// Ampel-LEDs setzen
void ampel_set(uint32_t leds)
{
    GPIOD->ODR = (GPIOD->ODR & ~ALLE_LEDS) | leds;
}

int main(void)
{
    gpio_init();
    systick_init();

    while (1)
    {
        // GRÜN: 4 Sekunden
        ampel_set(GRUEN);
        delay_ms(4000);

        // ORANGE: 1 Sekunde
        ampel_set(ORANGE);
        delay_ms(1000);

        // ROT: 4 Sekunden
        ampel_set(ROT);
        delay_ms(4000);

        // ROT + ORANGE: 1 Sekunde
        ampel_set(ROT | ORANGE);
        delay_ms(1000);
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
