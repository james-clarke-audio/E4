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
#ifndef BUTTON_H
#define BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"  // Device header

/*!
* @brief Multi-function and debounce button object
*/
class Button
{
private:
    uint16_t _IOPin;    // internal value of the button pin
    GPIO_TypeDef* _port;

public:
    Button(uint16_t pin, GPIO_TypeDef* port);   // constructor of class
    uint16_t GetIoPin();                        // returns IO pin of the button

    bool WasDown = false;                       // stores was down state as set by CaptureDownState()
    uint32_t RepeatCnt = 0;                     // maintains a count of number of times repeat trigger sent
    uint32_t LastRepeatMs = 0;                  // keeps a record of when last repeat trigger was sent

    bool IsDown();                              // does a debounce check to see if the button is in the down state
    bool IsUp();                                // does a debounce check to see if the button is in the up state
    bool CaptureDownState();                    // will set the WasDown flag true if it was pressed when checked
    bool ClearWasDown();                        // clears the WasDown state, returns true if clear was done
    bool PressRelesed();                        // provides a trigger action if the button was pressed, but now released
    bool LongPressed();                         // provides a trigger for long press case
    bool Repeated();                            // provides and action trigger at a higher frequency for as long as a key is held down
};

#ifdef __cplusplus
}
#endif

#endif // BUTTON_H