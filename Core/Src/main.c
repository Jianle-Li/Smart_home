/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "dht11.h"
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t Data[5];
//Gets the number of digits in decimal for each element in Data[].
int getDecimalDigits(uint8_t num)
{
    int count = 0;

    while (num > 0)
    {
        num /= 10;
        count++;
    }

    return count;
}
//Displays temperature and humidity
void dht_oled()
{
    if(DHT_Read())
    {
        OLED_ShowCHinese(0,0,8);//"shi"
        OLED_ShowCHinese(16,0,10);//"du"
        OLED_ShowChar(32,0,26+' ',16);//":"
        OLED_ShowNum(40,0,(unsigned int)Data[0],getDecimalDigits(Data[0]),16);//Humidity values in single digits and tens of digits
        OLED_ShowChar(40+getDecimalDigits(Data[0])*8,0,5+' ',16);//"%"
        //printf("H:%.1f%%,T:%.fC\r\n",Data[0]+0.1*Data[1],Data[2]+0.1*Data[3]);
        OLED_ShowCHinese(0,2,9);//"wen"
        OLED_ShowCHinese(16,2,10);//"du"
        OLED_ShowChar(32,2,26+' ',16);//":"
        OLED_ShowNum(40,2,(unsigned int)Data[2],getDecimalDigits(Data[2]),16);//Temperature values in single digits and tens
        OLED_ShowChar(40+getDecimalDigits(Data[2])*8,2,14+' ',16);//"."
        OLED_ShowNum(40+(getDecimalDigits(Data[2])+1)*8,2,(unsigned int)Data[3],getDecimalDigits(Data[3]),16);//The decimal place of the temperature value
        OLED_ShowChar(40+(getDecimalDigits(Data[2])+getDecimalDigits(Data[3])+1)*8,2,35+' ',16);//"C"
    }
    HAL_Delay(50);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  uint16_t pwmVal=500;//The higher the value,the dimmer the headlight
  OLED_Init();
  OLED_Clear();
  OLED_ShowCHinese(32,0,0);//"bi"
  OLED_ShowCHinese(48,0,1);//"ye"
  OLED_ShowCHinese(64,0,2);//"she"
  OLED_ShowCHinese(80,0,3);//"ji"
  OLED_ShowNum(24,2,3120002342,10,16);//"3120002342"
  OLED_ShowCHinese(32,4,4);//"zhi"
  OLED_ShowCHinese(48,4,5);//"neng"
  OLED_ShowCHinese(64,4,6);//"jia"
  OLED_ShowCHinese(80,4,7);//"ju"
  HAL_Delay(1000);
  OLED_Clear();
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  //__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,pwmVal);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      dht_oled();
      while(pwmVal){
          pwmVal--;
          __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,pwmVal);
          HAL_Delay(5);
      }
      while (pwmVal<500){
          pwmVal++;
          __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,pwmVal);
          HAL_Delay(5);
      }
      HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
