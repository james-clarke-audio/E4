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
#include "timer.h"
#include "encoder.h"

// Private forward function prototypes
void GPIO_Init();
void SystemClock_Config();
void Error_Handler_Main();
void CaptureButtonDownStates();
uint8_t getUint32_tCharCnt(uint32_t value);
uint8_t getFloat_CharCnt(float value, uint8_t places);
void printChars(uint8_t cnt, char c);
void printUint32_tAtWidth(uint32_t value, uint8_t width, char c, bool isRight);
void printFloat_AtWidth(float value, uint8_t width, char c, bool isRight);

// create object for OLED display
// see config-blackpill.h for pin definitions
Display display(SDA_PIN, SCL_PIN, I2C_MODULE_1); 

// Global Fonts used for application
FontDef_t Font_6x8 = {6, 8, Font6x8};

// Button Objects PIN definitions in config-blackpill.h
Button LeftButton(BUTTON_LEFT, GPIOA);
Button RightButton(BUTTON_RIGHT, GPIOA);

Timer t2(2, 0, TIM_COUNTERMODE_UP, 65535, TIM_CLOCKDIVISION_DIV1, TIM_AUTORELOAD_PRELOAD_DISABLE);
Timer t4(4, 0, TIM_COUNTERMODE_UP, 65535, TIM_CLOCKDIVISION_DIV1, TIM_AUTORELOAD_PRELOAD_DISABLE);

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
uint32_t cntL = 0;
uint32_t cntR = 0;

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

    // encoders (pass references to menu object to use wheels as up/down buttons)
    /* TIM2 GPIO Configuration
    PA15     ------> TIM2_CH1   --> CHA
    PB3     ------> TIM2_CH2    --> CHB
    */
    /* TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1    --> CHB
    PB7     ------> TIM4_CH2    --> CHA
    */
    Encoder leftWheel(t2.GetTimer(), GPIO_PIN_15, GPIO_PIN_3, GPIOA, GPIOB, GPIO_AF1_TIM2);
    Encoder rightWheel(t4.GetTimer(), GPIO_PIN_6, GPIO_PIN_7, GPIOB, GPIOB, GPIO_AF2_TIM4);

    // initialise encoders
    leftWheel.Init();
    rightWheel.Init();

    // menu system
    Menu menu(&LeftButton, &RightButton, &rightWheel, &display, Font_6x8);

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
  	display.GotoXY (5, 1);
    display.Print ("Welcome", Font_6x8, COLOR_WHITE);
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
        display.Print("L = ", Font_6x8, COLOR_WHITE);
        printUint32_tAtWidth(cntL, 10, ' ', false);      // cntL
        
        display.GotoXY (5,30);
        display.Print("R = ", Font_6x8, COLOR_WHITE);
        printUint32_tAtWidth(cntR, 10, ' ', false);

        display.GotoXY (5,40);
        display.Print("Angle = ", Font_6x8, COLOR_WHITE);
        printFloat_AtWidth(angleL, 10, ' ', false);

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

uint8_t getUint32_tCharCnt(uint32_t value){

    if (value==0){return 1;}
    uint32_t tensCalc = 10;
    uint8_t cnt = 1;

    while (tensCalc <= value && cnt < 20){
        tensCalc *= 10;
        cnt += 1;
    }

    return cnt;
}

uint8_t getFloat_CharCnt(float value, uint8_t places){

    if (value==0){return 1;}
    uint8_t cnt = 1;

    //float dum = 167.1234567;
    // two components to a float: integer part, decimal part (4 decimal places only maybe!)
    // integer component
    int tmpInt = value;
    int tmpNew = 0;
    cnt = getUint32_tCharCnt(tmpInt);   // this gets the length of the integer component

    // next we need to remove the integer component and leave the decimal
    // 0.1234567
    // iterate through decimal number by the number of decimal places asked for
    float tmpDec = value - tmpInt;

    for (int i=0; i < places; i++){
        tmpNew = tmpDec * 10;  //1.234567
        tmpDec = tmpDec - tmpNew;  //0.234567
        cnt++;
    }

    return ++cnt;   // need to include a decimal point
}

void printChars(uint8_t cnt, char c){
    if (cnt > 0){
        char cc[] = " "; cc[0] = c;
        for (uint8_t i = 1; i <= cnt; i++) {display.Print(cc, Font_6x8, COLOR_WHITE);}
    }
}

void printUint32_tAtWidth(uint32_t value, uint8_t width, char c, bool isRight){
    uint8_t numChars = getUint32_tCharCnt(value);
    char str[numChars];
    sprintf(str,"%d", value);

    if (isRight){printChars(width-numChars, c);}
    display.Print(str, Font_6x8, COLOR_WHITE);
    if (!isRight){printChars(width-numChars, c);}
}

void printFloat_AtWidth(float value, uint8_t width, char c, bool isRight){

    uint8_t numChars = getFloat_CharCnt(value, 4);
    char str[numChars];
    char dec[4];
    dec[0] = ' ';
    dec[1] = ' ';
    dec[2] = ' ';
    dec[3] = ' ';
    dec[4] = ' ';  //needed to prevent a dodgy character being displayed.  not sure why!!!

    int tmpInt1 = value;  // Get the integer (678).
    float tmpFrac = value - (float)tmpInt1; // Get fraction (0.0123).
    int tmpInt2 = tmpFrac*10000;

    if (isRight){printChars(width-numChars, c);}
    //print first part of number
    sprintf (str, "%d.", tmpInt1);
    display.Print(str, Font_6x8, COLOR_WHITE);

    for (int i = 0; i < 4; i++){
        // need to move the number up by 10 and print the actual number
        tmpFrac = tmpFrac * (float)10;  // 0.1 becomes 1.0 and so on
        tmpInt2 = tmpFrac;          // get the integer component
        dec[i] = (char)(tmpInt2+ '0');
        tmpFrac = tmpFrac - (float)tmpInt2;
    }
    display.Print(dec, Font_6x8, COLOR_WHITE);

    if (!isRight){printChars(width-numChars, c);}
    
}
