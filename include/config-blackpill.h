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

// User Buttons
#define BUTTON_LEFT GPIO_PIN_11
#define BUTTON_RIGHT GPIO_PIN_12

// I2C OLED Display
#define SCL_PIN GPIO_PIN_8
#define SDA_PIN GPIO_PIN_9
#define I2C_MODULE_1                  ((uint16_t)0x0001)
#define I2C_MODULE_2                  ((uint16_t)0x0002)

// Encoder Channels
#define LEFT_CHA GPIO_PIN_15
#define LEFT_CHB GPIO_PIN_3

#define RIGHT_CHA GPIO_PIN_7
#define RIGHT_CHB GPIO_PIN_6


// SPI Gyro and Accelerometer


#ifdef __cplusplus
}
#endif

#endif // CONFIG_BLACKPILL_H