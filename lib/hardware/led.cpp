/******************************************************************************
 * Project: stm32 - E4                                                        *
 * -------------------------------------------------------------------------- *
 * Copyright 2024 - James Clarke                                              *
 *                                                                            *
 * LED can be driven in two ways: logic low and logic high                    *
 * mode false:   vcc ---[led]--[resistor]--- IOPIN                            *
 *              setting IO pin LOW will turn led ON                           *
 *              setting IO pin HIGH will turn led OFF                         *
 * mode true:  IOPIN ----[resistor]--[led]---GND                              *
 *              setting IO pin HIGH will turn led ON                          *
 *              setting IO pin LOW will turn led OFF                          *
 * -------------------------------------------------------------------------- *
 * Licence:                                                                   *
 *     Use of this source code is governed by an MIT-style                    *
 *     license that can be found in the LICENSE file or at                    *
 *     https://opensource.org/licenses/MIT.                                   *
 ******************************************************************************/
#include "led.h"

// class constructor
Led::Led(uint16_t pin, GPIO_TypeDef* port, bool mode){

    // push constructor values to internal variables
    _IOPin = pin;
    _port = port;
    _mode = mode;

    if (port == GPIOA){
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB){
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC){
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    
    // we can initialise the Led here.
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(_port, _IOPin, GPIO_PIN_RESET);

    /*Configure GPIO pins */
    GPIO_InitStruct.Pin = _IOPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(_port, &GPIO_InitStruct);

}

// returns IO pin of the button 
uint16_t Led::GetIoPin(){return _IOPin;}

bool Led::GetMode(){return _mode;}

// turns LED on, returns true if action was done
bool Led::On(){
    if (_mode){
        // setting pin "true" turns led on
        HAL_GPIO_WritePin(_port, _IOPin, GPIO_PIN_SET);
    }
    else{
       // setting pin "true" turns led on
        HAL_GPIO_WritePin(_port, _IOPin, GPIO_PIN_RESET);
    }
    return true;
}

// turns LED off, returns true if action was done
bool Led::Off(){
    if (_mode){
        // setting pin "false" turns led off
        HAL_GPIO_WritePin(_port, _IOPin, GPIO_PIN_RESET);
    }
    else{
       // setting pin "false" turns led off
        HAL_GPIO_WritePin(_port, _IOPin, GPIO_PIN_SET);
    }
    return true;
}

// toggles LED based on previous state, returns true if action was done                    
bool Led::Toggle(){
    HAL_GPIO_TogglePin(_port, _IOPin);
    return true;
}                            