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

    #define ENC_ZERO (0)
    #define ENC_RESOLUTION (1024 - 1)

    TIM_HandleTypeDef htimer;
    void InitTimer(uint32_t alternate, uint32_t channela_pin, uint32_t channelb_pin, GPIO_TypeDef* channela_port, GPIO_TypeDef* channelb_port);
    void Error_Handler(void);

public:
    
    Encoder(TIM_HandleTypeDef timer, uint32_t channela_pin, uint32_t channelb_pin, GPIO_TypeDef* channela_port, GPIO_TypeDef* channelb_port, uint32_t alternate); // constructor
    void Init();
    uint16_t Read();
    void ResetCount();
    float GetAngle();

};

#ifdef __cplusplus
}
#endif

#endif // ENCODER_H