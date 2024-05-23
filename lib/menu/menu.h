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
#ifndef MENU_H
#define MENU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"  // Device header
#include <stdio.h>
#include "display.h"
#include "button.h"

/*!
* @brief Menu System Class
*/
class Menu {

    private:

    /******************************************************************************
     * Menu System Defines                                                        *
     ******************************************************************************/
    #define DISP_ITEM_ROWS 3
    #define DISP_CHAR_WIDTH 20
    #define PACING_MS 25
    #define FLASH_RST_CNT 30

    /******************************************************************************
     * Declarations                                                               *
     ******************************************************************************/
    // Menu ENUM and variables
    enum PageType {
        MENU_ROOT,
        MENU_MODE,
        MENU_SETUP,
        MENU_CALIBRATE,
        MENU_EXITMENU
    };
    enum PageType currPage = MENU_ROOT;

    uint32_t loopStartMs;
    bool updateAllItems;
    bool updateItemValue;
    uint8_t itemCnt;
    uint8_t pntrPos;
    uint8_t displayOffset;
    uint8_t flashCntr;
    bool flashIsOn;

    char screen_buffer[30];

    // Menu Page Function declarations
    void page_MenuRoot();
    void page_MenuMode();    
    void page_MenuSetUp();    
    void page_MenuCalibrate();

    void initMenuPage(const char title[], uint8_t itemCount);
    void captureButtonDownState();
    void adjustBoolean(bool *v);
    void adjustUint8_t(uint8_t *v, uint8_t min, uint8_t max);
    void doPointerNavigation();
    bool isFlashChanged();
    void pacingWait();
    bool menuItemPrintable(uint8_t xPos, uint8_t yPos);

    // print tools
    void printPointer();
    void printOffsetArrows();
    void printOnOff(bool val);
    void printUint32_tAtWidth(uint32_t value, uint8_t width, char c, bool isRight);

    // Hardware components for the menu
    Button* _leftbutton;
    Button* _rightbutton;
    Display* _display;
    FontDef_t _font;

    public:

    Menu(Button* leftbutton, Button* rightbutton, Display* display, FontDef_t font);   // constructor of class

    bool TestPrint(const char str[]); 
    // main entry point for class
    bool menu_Main();

};




/******************************************************************************
 * -------------------------------------------------------------------------- *
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // MENU_H