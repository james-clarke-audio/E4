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
#include "timer.h"

Timer::Timer(uint8_t channel, uint32_t prescaler, uint32_t countermode, uint32_t period, uint32_t clockdivision, uint32_t autoreload){

    // simple pass of the timer being created
    switch (channel){
        case 1: htimer.Instance = TIM1; break;
        case 2: htimer.Instance = TIM2; break;
        case 3: htimer.Instance = TIM3; break;
        case 4: htimer.Instance = TIM4; break;
        case 5: htimer.Instance = TIM5; break;
    }

    // initialise the timer
    htimer.Init.Prescaler = prescaler;
    htimer.Init.CounterMode = countermode;
    htimer.Init.Period = period;
    htimer.Init.ClockDivision = clockdivision;
    htimer.Init.AutoReloadPreload = autoreload;

}

void Timer::InitHardware(){

}

TIM_HandleTypeDef Timer::GetTimer(){
    return htimer;
}
