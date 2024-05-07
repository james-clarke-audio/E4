#pragma once
/******************************************************************************
 * Project: stm32 - E4                                                        *
 * -------------------------------------------------------------------------- *
 * Copyright 2024 - James Clarke                                              *
 * -------------------------------------------------------------------------- *
 * The config.h file determines the actual robot variant that will be the     *
 * target for the build.                                                      *
 *                                                                            *
 * This files lets you pick a specific robot that has its own unique          *
 * configuration stored in that file. In this way, you can write generic code *
 * that will work with a variety of different actual robot.                   *
 * -------------------------------------------------------------------------- *
 * Licence:                                                                   *
 *     Use of this source code is governed by an MIT-style                    *
 *     license that can be found in the LICENSE file or at                    *
 *     https://opensource.org/licenses/MIT.                                   *
 ******************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

/******************************************************************************
 * You may use a slightly different hardware platform than the BlackPill      *
 * Here you can include a suitable hardware configuration to define           *
 * things like IO pins, ADC channels and so on                                *
 ******************************************************************************/
#define HARDWARE_UNKNOWN 0
#define HARDWARE_BLACKPILL 1
#define HARDWARE_BLUEPILL 2
#define HARDWARE_ARDUINO 3

#define HARDWARE HARDWARE_BLACKPILL

#if HARDWARE == HARDWARE_BLACKPILL
#include "config-blackpill.h"
#elif HARDWARE == HARDWARE_BLUEPILL
#include "config-bluepill.h"
#else
#error "NO HARDWARE DEFINED"
#endif

/******************************************************************************
 * can have different sensor defaults for each of these environments so here  *
 * you can define which set will be used.                                     *
 ******************************************************************************/
#define EVENT_HOME 1
#define EVENT_UK 2
#define EVENT_PORTUGAL 3
#define EVENT_APEC 4

// choose the one you will be using BEFORE selecting the robot below
#define EVENT EVENT_HOME

#if EVENT == EVENT_HOME
#define GOAL Location(3, 3)
#else
#define GOAL Location(7, 7)
#endif

/******************************************************************************
 * Even with the same basic hardware, you may build robots with different     *
 * characteristics such as the motor gear ratio or the wheel size or sensor   *
 * arrangement.                                                               *
 *                                                                            *
 * These characteristics are kept in their own config files. Add your robot   *
 * to the list and create a corresponding config file with its custom values. *
 ******************************************************************************/
#define ROBOT_NOT_DEFINED 0
#define ROBOT_BLACKPILL_E4 1
#define ROBOT_BLUEPILL_E3 2
#define ROBOT_BLACKPILL_E4_V3 3

#define ROBOT ROBOT_BLACKPILL_E4

#if ROBOT == ROBOT_BLACKPILL_E4
#include "config-robot-E4.h"
#elif ROBOT == ROBOT_BLUEPILL_E3
#include "config-robot-E3.h"
#else
#error "NO ROBOT DEFINED"
#endif

/******************************************************************************
 * Universal constants                                                        *
 ******************************************************************************/
// This is the size, in mm,  for each cell in the maze.
#define FULL_CELL 180.0f
#define HALF_CELL (FULL_CELL / 2.0)

#define CODE "STM32F-BLACKPILL"
#define PI 3.14159
#define RADIANS_PER_DEGREE (2 * PI / 360.0)
#define DEGREES_PER_RADIAN (360.0 / 2 * PI)

#define MICROMETERS_PER_METER 1000000
#define MICROSECONDS_PER_SECOND 1000000

/** System clock frequency is set in `setup_clock` */
#define SYSCLK_FREQUENCY_HZ 72000000
#define SYSTICK_FREQUENCY_HZ 1000
#define DRIVER_PWM_PERIOD 1024

/******************************************************************************
 * Structure definitions used in the software.
 * Robot specific instances and values are in the robot config file.
 ******************************************************************************/
typedef struct TurnParameters {
  int speed;    // mm/s    - constant forward speed during turn
  int run_in;   // mm      - distance from cell edge to turn start
  int run_out;  // mm      - distance from turn end to cell start
  int angle;    // deg     - total turn angle
  int omega;    // deg/s   - maximum angular velocity
  int alpha;    // deg/s/s - angular acceleration
  int trigger;  //         - front sensor value at start of turn
} TurnParameters;

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H