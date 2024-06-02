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
Encoder::Encoder(TIM_HandleTypeDef timer, uint32_t channela_pin, uint32_t channelb_pin, GPIO_TypeDef* channela_port, GPIO_TypeDef* channelb_port, uint32_t alternate) {
    // push constructor values to internal variables
    htimer = timer;
    InitTimer(alternate, channela_pin, channelb_pin, channela_port, channelb_port);
}

void Encoder::Init(){ 
    HAL_TIM_Encoder_Start(&htimer, TIM_CHANNEL_ALL);
}

uint16_t Encoder::Read(){
    uint16_t l_return = 0;
    l_return = (uint16_t)htimer.Instance->CNT;
    return l_return;
}

void Encoder::ResetCount(){
    htimer.Instance->CNT = ENC_ZERO;
}

float Encoder::GetAngle(){
    float l_return = 0.0;
    l_return = (2 * 3.141 * (float)((int32_t)htimer.Instance->CNT - (int32_t)ENC_ZERO) / (float)ENC_RESOLUTION);
    return l_return;
}

/******************************************************************************
 * PRIVATE METHODS                                                            *
 *****************************************************************************/
void Encoder::InitTimer(uint32_t alternate, uint32_t channela_pin, uint32_t channelb_pin, GPIO_TypeDef* channela_port, GPIO_TypeDef* channelb_port){

    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // hardware first
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* TIM2 GPIO Configuration
    PA15     ------> TIM2_CH1   --> CHA
    PB3     ------> TIM2_CH2    --> CHB
    */

    /* TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1    --> CHB
    PB7     ------> TIM4_CH2    --> CHA
    */

    GPIO_InitStruct.Pin = channela_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = alternate;
    HAL_GPIO_Init(channela_port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = channelb_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = alternate;
    HAL_GPIO_Init(channelb_port, &GPIO_InitStruct);

    // now the timer
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;        //TIM_ICPOLARITY_RISING
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV4;              //TIM_ICPSC_DIV1
    sConfig.IC1Filter = 0;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;          // TIM_ICPOLARITY_BOTHEDGE
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV4;
    sConfig.IC2Filter = 0;
    if (HAL_TIM_Encoder_Init(&htimer, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htimer, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

void Encoder::Error_Handler(){
  __disable_irq();
  while (1)
  {
  }
}

