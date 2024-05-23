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
#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"  // Device header

/*!
* @brief Encoders Object - two wheels to drive
*/

class Encoder{

private:

    #define ENC_CNT_L (TIM2 -> CNT)
    #define ENC_CNT_R (TIM4 -> CNT)

    #define ENC_ZERO (20000)
    #define ENC_RESOLUTION (1024 - 1)

    uint8_t _timerChannel;
    TIM_HandleTypeDef htim2;
    TIM_HandleTypeDef htim4;

    void InitTimer2(void);
    void InitTimer4(void);

    void InitTimer2Hardware(void);
    void InitTimer4Hardware(void);
    void Error_Handler(void);

public:
    
    Encoder(uint8_t timer); // constructor

    void EncoderInit();
    uint16_t Read();
    void ResetCount();
    float GetAngle();

};

#ifdef __cplusplus
}
#endif

#endif // ENCODER_H