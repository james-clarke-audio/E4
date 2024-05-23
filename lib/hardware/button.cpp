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
#include "button.h"

 // constructor of class
Button::Button(uint16_t pin, GPIO_TypeDef* port){

    // push constructor values to internal variables
    _IOPin = pin;
    _port = port;

    // we can initialise the Button here.
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins*/
    GPIO_InitStruct.Pin = _IOPin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_port, &GPIO_InitStruct);
}

// returns IO pin of the button 
uint16_t Button::GetIoPin(){return _IOPin;}

// does a debounce check to see if the button is in the down state
bool Button::IsDown(){
    return ((HAL_GPIO_ReadPin(_port, _IOPin) == GPIO_PIN_SET) && (HAL_GPIO_ReadPin(_port, _IOPin) == GPIO_PIN_SET));
}                

// does a debounce check to see if the button is in the up state
bool Button::IsUp(){
    return ((HAL_GPIO_ReadPin(_port, _IOPin) == GPIO_PIN_RESET) && (HAL_GPIO_ReadPin(_port, _IOPin) == GPIO_PIN_RESET));
}      

// will set the WasDown flag true if it was pressed when checked
bool Button::CaptureDownState(){if (IsDown()){WasDown = true;} return WasDown;}

// clears the WasDown state, returns true if clear was done
bool Button::ClearWasDown(){
    if (WasDown){
        WasDown = false; 
        return true;
    } 
    return false;
}   

// provides a trigger action if the button was pressed, but now released
// clears WasDown state when trigger sent
bool Button::PressRelesed(){if (WasDown && IsUp()){RepeatCnt = 0; WasDown = false; return true;} return false;}            

// provides a trigger for long press case
bool Button::LongPressed(){return (Repeated() && RepeatCnt == 2);}        

// provides and action trigger at a higher frequency for as long as a key is held down
bool Button::Repeated(){
    uint32_t currMs = HAL_GetTick();

    if (WasDown && (
                    RepeatCnt == 0 ||
                    (RepeatCnt > 5 && currMs >= (LastRepeatMs + 200)) ||
                    currMs >= (LastRepeatMs + 250 + (50 * (5 - RepeatCnt)))
                    ))
    {
        // increase the repeat count limiting to max value 999 to avoid rollover
        if (RepeatCnt < 999) {RepeatCnt++;}
        // clear the was down state to get the next repeat
        WasDown = false;
        // take note of the last repeat time to be used in the next comparison & send back a positive trigger
        LastRepeatMs = currMs;
        return true;
    }
    else
    {
        // has repeated and button is now up, then clear the repeat count and was down state
        if (RepeatCnt > 0 && IsUp()){
            RepeatCnt = 0;
            WasDown = false;
        }
        return false;
    }

}                           

