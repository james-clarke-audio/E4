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
#include "config.h"
#include "classtest.h"
#include "display.h"

/* Private forward function prototypes */
void GPIO_Init(void);
void SystemClock_Config(void);
void Error_Handler_Main(void);

TestCPP TestObj(3);
Display display(0);

/* Global Font used for application */
FontDef_t Font_6x8 = {
    6,
    8,
    Font6x8
};

int main()
{
	  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	  HAL_Init();
  	SystemClock_Config();
	  GPIO_Init();
	  display.Init();  /* initialise LCD display on I2C bus */

	  /* Welcome to E4! */
  	display.GotoXY (5, 1);
    display.Puts ((char*)"Welcome to ECAC", &Font_6x8, (DISPLAY_COLOR_t)1);
    display.GotoXY (5, 8);
    display.Puts ((char*)"(c) James Clarke", &Font_6x8, (DISPLAY_COLOR_t)1);

    display.DrawRectangle (0, 0, 128, 15, (DISPLAY_COLOR_t)1);
    display.DrawRectangle (0, 16, 128, 63, (DISPLAY_COLOR_t)1);

    display.GotoXY (5,40);
    display.Puts ((char*)"Left Button Off", &Font_6x8, (DISPLAY_COLOR_t)1);
    display.GotoXY (5,50);
    display.Puts ((char*)"Right Button Off", &Font_6x8, (DISPLAY_COLOR_t)1);

    display.UpdateScreen(); //display

    /* Turn Leds Off */
    LED_LEFT_OFF();
  	LED_RIGHT_OFF();
  	LED_BLUE_OFF();

    while (1)
    {
        //button pressed
        if(SWITCH_LEFT())
        {
            LED_LEFT_ON();  //Turn LED ON
            display.GotoXY (5,40);
            display.Puts ((char*)"Left Button On ", &Font_6x8, (DISPLAY_COLOR_t)0);
            display.UpdateScreen(); //display
        }
        else
        {
            LED_LEFT_OFF();  //Turn LED OFF
            display.GotoXY (5,40);
            display.Puts ((char*)"Left Button Off", &Font_6x8, (DISPLAY_COLOR_t)1);
            display.UpdateScreen(); //display
        }
        //button pressed
        if(SWITCH_RIGHT())
        {
            LED_RIGHT_ON();  //Turn LED ON
            display.GotoXY (5,50);
            display.Puts ((char*)"Right Button On ", &Font_6x8, (DISPLAY_COLOR_t)0);
            display.UpdateScreen(); //display
        }
        else
        {
            LED_RIGHT_OFF();  //Turn LED OFF
            display.GotoXY (5,50);
            display.Puts ((char*)"Right Button Off", &Font_6x8, (DISPLAY_COLOR_t)1);
            display.UpdateScreen(); //display
        }
    }
}

extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}

void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_ONBOARD|IR_SIDE_RIGHT|IR_FRONT_RIGHT, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_RIGHT|LED_LEFT|GPIO_PIN_12|IR_FRONT_LEFT|IR_SIDE_LEFT, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = LED_ONBOARD|IR_SIDE_RIGHT|IR_FRONT_RIGHT;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = LED_RIGHT|LED_LEFT|GPIO_PIN_12|IR_FRONT_LEFT|IR_SIDE_LEFT;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins*/
  GPIO_InitStruct.Pin = BUTTON_LEFT|BUTTON_RIGHT;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /******************************************************************************/
  /* Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure. */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler_Main();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler_Main();
  }
}

void Error_Handler_Main(void)
{
  __disable_irq();
  while (1)
  {
   	HAL_GPIO_TogglePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD);
	HAL_Delay(100*1); 
  }
}
