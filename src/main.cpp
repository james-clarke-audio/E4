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
#include <stdio.h>
#include "display.h"
#include "button.h"
#include "led.h"
#include "menu.h"
#include "encoder.h"

/* Private forward function prototypes */
void GPIO_Init();
void SystemClock_Config();
void Error_Handler_Main();
void CaptureButtonDownStates();

// create object for OLED display
// see config-blackpill.h for pin definitions
Display display(SDA_PIN, SCL_PIN, I2C_MODULE_1); 

// Global Fonts used for application
FontDef_t Font_6x8 = {6, 8, Font6x8};

// Button Objects PIN definitions in config-blackpill.h
Button LeftButton(BUTTON_LEFT, GPIOA);
Button RightButton(BUTTON_RIGHT, GPIOA);

Encoder leftWheel(2);
Encoder rightWheel(4);

// LED Objects PIN definitions in config-blackpill.h
Led LedLeft(LED_LEFT, GPIOB, true);
Led LedRight(LED_RIGHT, GPIOB, true);
Led LedOnboard(LED_ONBOARD, GPIOC, false);

// IR Emitter Objects PIN definitions in config-blackpill.h
Led IRSideRight(IR_SIDE_RIGHT, GPIOC, true);
Led IRFrontRight(IR_FRONT_RIGHT, GPIOC, true);
Led IRFrontLeft(IR_FRONT_LEFT, GPIOB, true);
Led IRSideLeft(IR_SIDE_LEFT, GPIOB, true);

// global score variables (TODO : Move to local scope to clean this up)
char zz[30];
uint16_t cntL = 0;
uint16_t cntR = 0;

float angleL = 0.0;
float angleR = 0.0;

int main()
{
    // set some internal local variables
    int i = 0;
    int dvalue = 0;

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
  	SystemClock_Config();

    // initialise the buttons / leds (slowly being deprecated as fucntionality moved to buttons and leds classes)
    GPIO_Init();

    // initialise LCD display
	display.Init();  

    // need to late bind some objects
    //Menu menu(&LeftButton, &RightButton, &display, Font_6x8);


    // initialise encoders
    leftWheel.EncoderInit();
    rightWheel.EncoderInit();

    // clear all button down states
    LeftButton.ClearWasDown();
    RightButton.ClearWasDown();

    /* Turn Leds Off */
    LedLeft.Off();
    LedRight.Off();
    LedOnboard.Off();

    // clear the button down states
    LeftButton.ClearWasDown();
    RightButton.ClearWasDown();

    // activate the main menu etc
    //menu.menu_Main();

	/* Welcome to E4! */
    display.Clear();
  	display.GotoXY (5, 0);
    display.Print ("Welcome", Font_6x8, COLOR_WHITE);
    display.GotoXY (5, 8);
    display.Print ("(c) James Clarke", Font_6x8, COLOR_WHITE);

    display.DrawRectangle (0, 0, 127, 15, COLOR_WHITE);
    display.DrawRectangle (0, 16, 127, 47, COLOR_WHITE);
    display.UpdateScreen(); //display

    while (1)
    {
        i++;
        CaptureButtonDownStates();

        angleL = leftWheel.GetAngle();
		angleR = rightWheel.GetAngle();

        cntL = leftWheel.Read();
        cntR = rightWheel.Read();


        display.GotoXY (5,20);
        sprintf(zz,"R = %d", cntL);
        display.Print (zz, Font_6x8, COLOR_WHITE);

        display.GotoXY (5,30);
        sprintf(zz,"L = %d", cntR);
        display.Print (zz, Font_6x8, COLOR_WHITE);

        display.GotoXY (5,40);
        sprintf(zz,"dummY = %d", dvalue);
        display.Print (zz, Font_6x8, COLOR_BLACK);

        display.UpdateScreen(); //display"

        // press the left button
        if (LeftButton.Repeated()){
            dvalue++;
            //LED_RIGHT_ON();
        }

        if (RightButton.PressRelesed()){
            // toggle leds
            LedOnboard.Toggle();

        }

    }
}

extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}

void GPIO_Init()
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

void SystemClock_Config()
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

void Error_Handler_Main()
{
    __disable_irq();
    while (1)
    {
        LedOnboard.Toggle();
        HAL_Delay(500*1); 
    }
}

void CaptureButtonDownStates(){
    LeftButton.CaptureDownState();
    RightButton.CaptureDownState();
}