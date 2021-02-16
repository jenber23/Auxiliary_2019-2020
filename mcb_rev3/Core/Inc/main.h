/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DEBUG_OUT_Pin GPIO_PIN_5
#define DEBUG_OUT_GPIO_Port GPIOA
#define PRE_MOTOR_Pin GPIO_PIN_6
#define PRE_MOTOR_GPIO_Port GPIOA
#define PRE_MPPT_Pin GPIO_PIN_7
#define PRE_MPPT_GPIO_Port GPIOA
#define CON_MOTOR_Pin GPIO_PIN_4
#define CON_MOTOR_GPIO_Port GPIOC
#define CON_MPPT_Pin GPIO_PIN_5
#define CON_MPPT_GPIO_Port GPIOC
#define CHG_TRIP_Pin GPIO_PIN_0
#define CHG_TRIP_GPIO_Port GPIOB
#define CHG_TRIP_EXTI_IRQn EXTI0_1_IRQn
#define CRUISE_IN_Pin GPIO_PIN_1
#define CRUISE_IN_GPIO_Port GPIOB
#define CRUISE_IN_EXTI_IRQn EXTI0_1_IRQn
#define REGEN_BRK_Pin GPIO_PIN_10
#define REGEN_BRK_GPIO_Port GPIOB
#define SS_CRUISE_Pin GPIO_PIN_9
#define SS_CRUISE_GPIO_Port GPIOA
#define SS_REGEN_Pin GPIO_PIN_10
#define SS_REGEN_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/