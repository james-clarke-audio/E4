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
#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"  // Device header

/*!
* @brief LED driving output object
*/
class Led
{
private:
    uint16_t _IOPin;    // internal value of the button pin
    GPIO_TypeDef* _port;
    bool _mode;

public:
    Led(uint16_t pin, GPIO_TypeDef* port, bool mode);     // class constructor
    uint16_t GetIoPin();                        // returns IO pin of the led
    bool GetMode();                             // returns mode of the led

    bool On();              // turns LED on
    bool Off();             // turns LED off
    bool Toggle();          // toggles LED based on previous state
};

#ifdef __cplusplus
}
#endif

#endif // LED_H