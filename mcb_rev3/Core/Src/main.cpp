/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "subsystem-data-module.hpp"
#include "aux-data-module.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/**
 * Enumeration for the SPI slave devices this uC will communicate with
 * 	- cruiseDAC is a LTC2630ISC6-LM12
 * 	- regenDAC is a LTC2630ISC6-LM12
 */
typedef enum slave
{
	cruiseDAC = 0,
	regenDAC = 1,
} slave_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

// CAN-related objects and variables
static AUX_MESSAGE_0_DATA_PACKET aux0Packet;
static AUX_MESSAGE_0 aux0;

// ISR-affected objects and variables
static bool active_CT = false;
static bool active_Cruise = false;

static bool newInput_CT	= false;
static bool newInput_Cruise = false;

static bool sysPrecharge = false;
static bool sysMCCoilActive = false;
static bool sysMPPTCoilActive = false;

static int tickPrecharge = 0;
static int tickCoil = 0;

static bool newInput_CAN = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

// ----------------------------
// --- ISR PROTOTYPE(S) -------
// ----------------------------
/**
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN);
/**
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
/**
 *
 */
void AUX_MotherReceive_Callback(SUBSYSTEM_DATA_MODULE*);

// ---------------------------------
// --- FUNCTION PROTOTYPE(S) -------
// ---------------------------------
/**
 *
 */
int DAC_Write(slave_t slave, uint8_t *data);
/**
 *
 */
int DAC_SpliceWrite(slave_t slave, uint8_t command, uint16_t *data);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	// DAC Commands
	static uint8_t DAC_Init[3]		= {0x70, 0x00, 0x00};	// Set external voltage reference (5V)
//	static uint8_t DAC_PowerOn[3]	= {0x30, 0xFF, 0xF0};	// Power on DAC with max output voltage
															// ! USE ONLY FOR TESTING DAC !
															// ! DO NOT USE ON FINAL BOARD !
	static uint8_t DAC_PowerOn[3]	= {0x30, 0x00, 0x00};	// Power on DAC
	static uint8_t DAC_PowerOff[3]	= {0x40, 0x00, 0x00};	// Power off DAC

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
  MX_SPI2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 47999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PRE_MOTOR_Pin|PRE_MPPT_Pin|SS_CRUISE_Pin|SS_REGEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CON_MOTOR_Pin|CON_MPPT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(REGEN_BRK_GPIO_Port, REGEN_BRK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PRE_MOTOR_Pin PRE_MPPT_Pin SS_CRUISE_Pin SS_REGEN_Pin */
  GPIO_InitStruct.Pin = PRE_MOTOR_Pin|PRE_MPPT_Pin|SS_CRUISE_Pin|SS_REGEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CON_MOTOR_Pin CON_MPPT_Pin */
  GPIO_InitStruct.Pin = CON_MOTOR_Pin|CON_MPPT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : CHG_TRIP_Pin */
  GPIO_InitStruct.Pin = CHG_TRIP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(CHG_TRIP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CRUISE_IN_Pin */
  GPIO_InitStruct.Pin = CRUISE_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CRUISE_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : REGEN_BRK_Pin */
  GPIO_InitStruct.Pin = REGEN_BRK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(REGEN_BRK_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// -----------------------------
// --- ISR DEFINITION(S) -------
// -----------------------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
	// check which pin triggered interrupt
	switch(GPIO_PIN)
	{
	case CHG_TRIP_Pin:

		break;
	case CRUISE_IN_Pin:

		break;
	default:
		__NOP();	// no operation
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// check which timer triggered interrupt
	switch(htim->Instance)
	{
	case TIM2:
		break;
	default:
		__NOP();	// no operation
	}
}
void AUX_MotherReceive_Callback(SUBSYSTEM_DATA_MODULE*)
{
	if(!aux0.isFifoEmpty())
		aux0Packet = aux0.GetOldestDataPacket(&newInput_CAN);
}

// ----------------------------------
// --- FUNCTION DEFINITION(S) -------
// ----------------------------------
int DAC_Write(slave_t slave, uint8_t *data)
{
	GPIO_TypeDef* currentPort;
	uint16_t currentPin;

	// identify which slave select to use for this transaction
	switch(slave)
	{
	case cruiseDAC:
		currentPort = SS_CRUISE_GPIO_Port;
		currentPin = SS_CRUISE_Pin;
		break;
	case regenDAC:
		currentPort = SS_REGEN_GPIO_Port;
		currentPin = SS_REGEN_Pin;
		break;
	default:
		// invalid slave select
		return -1;	// unsuccessful transaction
		break;
	}

	// pull slave select low
	HAL_GPIO_WritePin(currentPort, currentPin, GPIO_PIN_RESET);
	// transmit an array of 3 8-bit values
	HAL_SPI_Transmit(&hspi2, data, 3, HAL_MAX_DELAY);
	// pull slave select high
	HAL_GPIO_WritePin(currentPort, currentPin, GPIO_PIN_SET);
	// poll for SPI ready
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);

	return 0;	// successful transaction
}
int DAC_SpliceWrite(slave_t slave, uint8_t command, uint16_t *data)
{
	uint8_t temp[3] = {0};				// initialize an array of size 3 to 0

	// splice the data into DAC's required format
	temp[0] = command;					// first 8 bits is command and 4 don't care bits
	temp[1] = ((*data) >> 4) & 0xFF;	// second 8 bits is data[11:4]
	temp[2] = ((*data) << 4) & 0xFF;	// third 8 bits is data[3:0] in the upper nibble
										// and 4 don't care bits in the lower nibble
	// write spliced data to DAC
	return DAC_Write(slave, temp);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/