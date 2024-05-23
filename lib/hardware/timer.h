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
#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"  // Device header

class Timer {

private:

    TIM_HandleTypeDef htimer;

public:

    Timer(uint8_t channel, uint32_t prescaler, uint32_t countermode, uint32_t period, uint32_t clockdivision, uint32_t autoreload);
    void InitHardware();
    TIM_HandleTypeDef GetTimer();

};

#ifdef __cplusplus
}
#endif

#endif // TIMER_H