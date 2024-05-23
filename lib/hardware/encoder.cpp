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
 *****************************************************************************/
#include "encoder.h"

/******************************************************************************
 * PUBLIC INTERFACE                                                           *
 *****************************************************************************/
// constructor of class
Encoder::Encoder(uint8_t timer) {

    // push constructor values to internal variables
    _timerChannel = timer;

    // pins for the encoders come in channela and channelb
    if (_timerChannel == 2){
        InitTimer2();
        // Init the low level hardware
        InitTimer2Hardware();
    }

    if (_timerChannel == 4){
        InitTimer4();
        // Init the low level hardware
        InitTimer4Hardware();
    }

    __enable_irq();

}

void Encoder::EncoderInit(){
    if (_timerChannel == 2){
        HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    } else if (_timerChannel == 4){
        HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    }
}

uint16_t Encoder::Read(){
    uint16_t l_return = 0;
    if (_timerChannel == 2){
        l_return = (uint16_t)ENC_CNT_L;
    }
    if (_timerChannel == 4){
        l_return = (uint16_t)ENC_CNT_R;
    }    
    return l_return;
}

void Encoder::ResetCount(){
    if (_timerChannel == 2){
        ENC_CNT_L = ENC_ZERO;
    }
    if (_timerChannel == 4){
        ENC_CNT_R = ENC_ZERO;        
    }
}

float Encoder::GetAngle(){
    float l_return = 0.0;
    if (_timerChannel == 2){
        l_return = (2 * 3.141 * (float)((int32_t)ENC_CNT_L - (int32_t)ENC_ZERO) / (float)ENC_RESOLUTION);
    }
    if (_timerChannel == 4){
        l_return = (2 * 3.141 * (float)((int32_t)ENC_ZERO - (int32_t)ENC_CNT_R) / (float)ENC_RESOLUTION);
    }
    return l_return;
}


/******************************************************************************
 * PRIVATE METHODS                                                            *
 *****************************************************************************/
void Encoder::InitTimer2() {

    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 65535;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 0;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 0;
    if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

}

void Encoder::InitTimer4() {

    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 0;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 65535;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 0;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 0;
    if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

}

void Encoder::InitTimer2Hardware(){

    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* TIM2 GPIO Configuration
    PA15     ------> TIM2_CH1   --> CHA
    PB3     ------> TIM2_CH2    --> CHB
    */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;   // GPIO_NOPULL
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void Encoder::InitTimer4Hardware(){

    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1    --> CHB
    PB7     ------> TIM4_CH2    --> CHA
    */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void Encoder::Error_Handler(){
  __disable_irq();
  while (1)
  {
  }
}

