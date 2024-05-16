#pragma once
/******************************************************************************
 * Project: stm32 - E4                                                        *
 * -------------------------------------------------------------------------- *
 * Copyright 2024 - James Clarke                                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Licence:                                                                   *
 *     Use of this source code is governed by an MIT-style                    *
 *     license that can be found in the LICENSE file or at                    *
 *     https://opensource.org/licenses/MIT.                                   *
 ******************************************************************************/
#ifndef CONFIG_BLACKPILL_H
#define CONFIG_BLACKPILL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

/******************************************************************************
 * hardware configuration for for stm32f blackpill robot. non stm32f hardware *
 * constants are defined in the appropriate config-robot file                 *
 ******************************************************************************/
// Encoders
const uint8_t ENCODER_LEFT_CLK = 2;
const uint8_t ENCODER_RIGHT_CLK = 3;
const uint8_t ENCODER_LEFT_B = 4;
const uint8_t ENCODER_RIGHT_B = 5;

// Motor Control
const uint8_t MOTOR_LEFT_DIR = 7;
const uint8_t MOTOR_RIGHT_DIR = 8;
const uint8_t MOTOR_LEFT_PWM = 9;
const uint8_t MOTOR_RIGHT_PWM = 10;

// Sensor Control
#define IR_SIDE_RIGHT GPIO_PIN_14
#define IR_FRONT_RIGHT GPIO_PIN_15
#define IR_FRONT_LEFT GPIO_PIN_4
#define IR_SIDE_LEFT GPIO_PIN_5

// LED Indicators
#define LED_RIGHT GPIO_PIN_2
#define LED_LEFT GPIO_PIN_10
#define LED_ONBOARD GPIO_PIN_13

#define LED_LEFT_GPIO_Port GPIOB 
#define LED_RIGHT_GPIO_Port GPIOB
#define LED_ONBOARD_GPIO_Port GPIOC

// User Buttons
#define BUTTON_LEFT GPIO_PIN_11
#define BUTTON_RIGHT GPIO_PIN_12

/* I2C OLED Display */
#define SCL_PIN GPIO_PIN_8
#define SDA_PIN GPIO_PIN_9
#define I2C_MODULE 1

/* SPI Gyro and Accelerometer */




/* useful macros */
#define LED_LEFT_ON()			HAL_GPIO_WritePin(LED_LEFT_GPIO_Port, LED_LEFT, GPIO_PIN_SET)
#define LED_LEFT_OFF()			HAL_GPIO_WritePin(LED_LEFT_GPIO_Port, LED_LEFT, GPIO_PIN_RESET)
#define LED_LEFT_TOGGLE()		HAL_GPIO_TogglePin(LED_LEFT_GPIO_Port, LED_LEFT)
#define LED_RIGHT_ON()			HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT, GPIO_PIN_SET)
#define LED_RIGHT_OFF()			HAL_GPIO_WritePin(LED_RIGHT_GPIO_Port, LED_RIGHT, GPIO_PIN_RESET)
#define LED_RIGHT_TOGGLE()		HAL_GPIO_TogglePin(LED_RIGHT_GPIO_Port, LED_RIGHT)
#define LED_BLUE_ON()			HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD, GPIO_PIN_RESET)
#define LED_BLUE_OFF()			HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD, GPIO_PIN_SET)
#define LED_BLUE_TOGGLE()		HAL_GPIO_TogglePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD)

#define LED_ALL_ON()			HAL_GPIO_WritePin (GPIOB, LED_LEFT|LED_RIGHT, GPIO_PIN_SET)
#define LED_ALL_OFF()			HAL_GPIO_WritePin (GPIOB, LED_LEFT|LED_RIGHT, GPIO_PIN_RESET)
#define LED_ALL_TOGGLE()		HAL_GPIO_TogglePin(GPIOB, LED_LEFT|LED_RIGHT)

/* SWITCHES */
#define SWITCH_LEFT()			HAL_GPIO_ReadPin(GPIOA, BUTTON_LEFT)
#define SWITCH_RIGHT()			HAL_GPIO_ReadPin(GPIOA, BUTTON_RIGHT)



#ifdef __cplusplus
}
#endif

#endif // CONFIG_BLACKPILL_H