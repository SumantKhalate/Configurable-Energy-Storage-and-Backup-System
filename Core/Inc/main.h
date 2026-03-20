/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

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
#define CLK_8MHZ_Pin GPIO_PIN_0
#define CLK_8MHZ_GPIO_Port GPIOH
#define CHARGER_ADC_VIN1_USB_Pin GPIO_PIN_0
#define CHARGER_ADC_VIN1_USB_GPIO_Port GPIOC
#define CHARGER_ADC_VIN2_Pin GPIO_PIN_1
#define CHARGER_ADC_VIN2_GPIO_Port GPIOC
#define CHARGER_ADC_SYS_Pin GPIO_PIN_2
#define CHARGER_ADC_SYS_GPIO_Port GPIOC
#define CONVERTER_ADC_CONV_VOUT_Pin GPIO_PIN_3
#define CONVERTER_ADC_CONV_VOUT_GPIO_Port GPIOC
#define USB_ADC_VUSB_Pin GPIO_PIN_0
#define USB_ADC_VUSB_GPIO_Port GPIOA
#define USB_ADC_VUSB_SW_Pin GPIO_PIN_1
#define USB_ADC_VUSB_SW_GPIO_Port GPIOA
#define CONVERTER_EN_Pin GPIO_PIN_3
#define CONVERTER_EN_GPIO_Port GPIOA
#define CONVERTER_INT_Pin GPIO_PIN_4
#define CONVERTER_INT_GPIO_Port GPIOA
#define CONVERTER_INT_EXTI_IRQn EXTI4_IRQn
#define CHARGER_QON_Pin GPIO_PIN_0
#define CHARGER_QON_GPIO_Port GPIOB
#define CHARGER_CE_Pin GPIO_PIN_1
#define CHARGER_CE_GPIO_Port GPIOB
#define CHARGER_INT_Pin GPIO_PIN_2
#define CHARGER_INT_GPIO_Port GPIOB
#define CHARGER_INT_EXTI_IRQn EXTI2_IRQn
#define I2C4_SCL_CHARGER_CONVERTER_Pin GPIO_PIN_10
#define I2C4_SCL_CHARGER_CONVERTER_GPIO_Port GPIOB
#define I2C4_SDA_CHARGER_CONVERTER_Pin GPIO_PIN_11
#define I2C4_SDA_CHARGER_CONVERTER_GPIO_Port GPIOB
#define I2C2_SCL_USB_GAUGE_Pin GPIO_PIN_13
#define I2C2_SCL_USB_GAUGE_GPIO_Port GPIOB
#define I2C2_SDA_USB_GAUGE_Pin GPIO_PIN_14
#define I2C2_SDA_USB_GAUGE_GPIO_Port GPIOB
#define USB_RESET_Pin GPIO_PIN_6
#define USB_RESET_GPIO_Port GPIOC
#define USB_ATTACH_Pin GPIO_PIN_7
#define USB_ATTACH_GPIO_Port GPIOC
#define USB_GPIO_Pin GPIO_PIN_8
#define USB_GPIO_GPIO_Port GPIOC
#define USB_ALERT_Pin GPIO_PIN_9
#define USB_ALERT_GPIO_Port GPIOC
#define USB_ALERT_EXTI_IRQn EXTI9_5_IRQn
#define UI_BUZZER_Pin GPIO_PIN_8
#define UI_BUZZER_GPIO_Port GPIOA
#define DEDUG_TX_Pin GPIO_PIN_9
#define DEDUG_TX_GPIO_Port GPIOA
#define DEDUG_RX_Pin GPIO_PIN_10
#define DEDUG_RX_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define SWDIO_TMS_Pin GPIO_PIN_13
#define SWDIO_TMS_GPIO_Port GPIOA
#define SWCLK_TCK_Pin GPIO_PIN_14
#define SWCLK_TCK_GPIO_Port GPIOA
#define USART3_TX_HOST_Pin GPIO_PIN_10
#define USART3_TX_HOST_GPIO_Port GPIOC
#define USART3_RX_HOST_Pin GPIO_PIN_11
#define USART3_RX_HOST_GPIO_Port GPIOC
#define UI_HEARTBEAT_Pin GPIO_PIN_2
#define UI_HEARTBEAT_GPIO_Port GPIOD
#define SWO_TDO_Pin GPIO_PIN_3
#define SWO_TDO_GPIO_Port GPIOB
#define I2C1_SCL_HOST_Pin GPIO_PIN_8
#define I2C1_SCL_HOST_GPIO_Port GPIOB
#define I2C1_SDA_HOST_Pin GPIO_PIN_9
#define I2C1_SDA_HOST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
