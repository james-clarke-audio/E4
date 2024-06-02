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
#include "menu.h"

// constructor of class
Menu::Menu(Button* leftbutton, Button* rightbutton, Encoder* wheel, Display* display, FontDef_t font){
    _leftbutton = leftbutton;
    _rightbutton = rightbutton;
    _wheel = wheel;
    _display = display;
    _font = font;
}

bool Menu::TestPrint(const char str[]){
    _display->GotoXY (5,40);
    sprintf(screen_buffer,"= %d", strlen(str));
    _display->Print(str, _font, COLOR_WHITE);
    _display->Print(screen_buffer, _font, COLOR_WHITE);
    _display->UpdateScreen();
    return true;
}

bool Menu::menu_Main() {
    bool exitMenu = false;
    while (!exitMenu) {
        switch(currPage){
            case MENU_ROOT: page_MenuRoot(); break;
            case MENU_MODE: page_MenuMode(); break;
            case MENU_SETUP: page_MenuSetUp(); break;
            case MENU_CALIBRATE: page_MenuCalibrate(); break;
            case MENU_EXITMENU: exitMenu = true; break;
        }
    }
    return true;
}

// Menu Page Function declarations
void Menu::page_MenuRoot(){

    //initialise root menu
    initMenuPage("Main", 4);

    while (1) {
        // print the display items when requested
        if (updateAllItems){
            
            // print the visible menu items
            if (menuItemPrintable(1,1)){
                _display->Print("Mode       ", _font, COLOR_WHITE); _display->UpdateScreen();
            }
            if (menuItemPrintable(1,2)){
                _display->Print("Setup      ", _font, COLOR_WHITE); _display->UpdateScreen();
            }
            if (menuItemPrintable(1,3)){
                _display->Print("Calibrate  ", _font, COLOR_WHITE); _display->UpdateScreen();
            }
            if (menuItemPrintable(1,4)){
                _display->Print("Exit Menu  ", _font, COLOR_WHITE); _display->UpdateScreen();
            }

            printOffsetArrows();
        }

        if (isFlashChanged()){printPointer();}

        // always clear update flag by this point
        updateAllItems = false;

        // capture the button down states
        captureButtonDownState();

        if (_rightbutton->PressRelesed()){
            switch (pntrPos){
                case 1 : currPage = MENU_MODE; return;
                case 2 : currPage = MENU_SETUP; return;
                case 3 : currPage = MENU_CALIBRATE; return;
                case 4 : currPage = MENU_EXITMENU; return;
            }
        }

        // otherwise check for the menu move button (circular 1,2,3,4,1,2,3,4,1,2...)
        doPointerNavigation();

        pacingWait();
    }
}

void Menu::page_MenuMode(){

    //initialise root menu
    initMenuPage("Mouse Mode", 3);

    while (1) {}
}

void Menu::page_MenuSetUp(){

    //initialise root menu
    initMenuPage("Mouse Setup", 3);

    while (1) {}
}    

void Menu::page_MenuCalibrate(){

    //initialise root menu
    initMenuPage("Mouse Calibration", 3);

    while (1) {}
}

void Menu::initMenuPage(const char title[], uint8_t itemCount){

    // clear the display
    _display->Clear();
    _display->GotoXY(5,4);

    //0..15 top section
    //16..63 bottom section

    //01234567890123456789  (example of what we are looking for)
    //=====MAIN MENU======

    uint8_t fillCnt = (DISP_CHAR_WIDTH - strlen(title)) / 2;
    if (fillCnt > 0) {
        for (uint8_t i = 0; i < fillCnt; i++){
            _display->Print("=", _font, COLOR_WHITE);
        }
    }
    _display->Print(title, _font, COLOR_WHITE);
    if ((strlen(title) % 2) == 1){fillCnt++;}
    if (fillCnt > 0) {
        for (uint8_t i = 0; i < fillCnt; i++){
            _display->Print("=", _font, COLOR_WHITE);
        }
    }

    // clear all button down states
    _leftbutton->ClearWasDown();
    _rightbutton->ClearWasDown();

    // set the menu item count
    itemCnt = itemCount;

    // current pointer position
    pntrPos = 1;

    // display offset
    displayOffset = 0;

    // flash counter > force immediate draw at startup
    flashCntr = 0;

    // flash state >  will switch to being on at startup
    flashIsOn = false;

    // force a full update
    updateAllItems = true;

    // place borders around screen sections (yellow / blue)
    _display->DrawRectangle (0, 0, 127, 15, COLOR_WHITE);
    _display->DrawRectangle (0, 16, 127, 47, COLOR_WHITE);
    _display->UpdateScreen(); //display

    // capture start time
    loopStartMs = HAL_GetTick();
}

void Menu::captureButtonDownState(){
    _leftbutton->CaptureDownState();
    _rightbutton->CaptureDownState();
}

void Menu::adjustBoolean(bool *v){
    while (1) {}
}

void Menu::adjustUint8_t(uint8_t *v, uint8_t min, uint8_t max){
    while (1) {}
}

void Menu::doPointerNavigation(){

    // this is also where we can use the encoders to control the navigation as well




    // check the button press
    if (_leftbutton->PressRelesed()){
        // erase the current pointer & reset the flash rate to get immediate redraw
        flashIsOn = false;
        flashCntr = 0;
        printPointer();

        if (pntrPos - displayOffset == DISP_ITEM_ROWS) {
            updateAllItems = true;
            displayOffset++;
        }

        // move the pointer
        pntrPos++;

        // as we are implementing a circular menu, need to reset the pntrPos to zero
        // once maximum has been reached
        if (pntrPos > itemCnt){
            // current pointer position
            pntrPos = 1;
            // display offset
            displayOffset = 0;
        }

    }
}

bool Menu::isFlashChanged(){
    if (flashCntr == 0){
        flashIsOn = !flashIsOn;
        flashCntr = FLASH_RST_CNT;
        return true;
    } else {
        flashCntr--;
        return false;
    }
}

void Menu::pacingWait(){
    // do the pacing wait
    while (HAL_GetTick() - loopStartMs < PACING_MS){
        HAL_Delay(1);
    }

    //capture start time
    loopStartMs = HAL_GetTick();
}

bool Menu::menuItemPrintable(uint8_t xPos, uint8_t yPos){

    // basic check to see if the basic conditions to allow showing this item
    if (!(updateAllItems || (updateItemValue && pntrPos == yPos))){ return false;}

    // make a value to use later to check if there is enough offset to make the value visible
    uint8_t yMaxOffset = 0;

    // need to convert the xPos and yPos to pixels for setting cursor position correctly
    // xPos = 0 correlates to pixel = 5, xPos 1 => 11 and so on i.e (xPos * 6) + 5
    // yPos = 0 correlates to pixel = 20, yPos 1 => 36 and so on i.e (yPos * 8) + 12
    uint8_t yPixels = (yPos * _font.FontHeight) + 12;
    uint8_t xPixels = (xPos * _font.FontWidth) + 5;

    // this case means the value is typically beyond the offset display, so we remove the visible row count
    if (yPos > DISP_ITEM_ROWS) {yMaxOffset = yPos - DISP_ITEM_ROWS;}

    //talking into account any offset, check if the item position is currently visible
    //position cursor and return true if so
    if (displayOffset <= (yPos-1) && displayOffset >= yMaxOffset)
    {
        // yPos = 0 sets the pixel point to 3
        // yPos = 1 sets the pixel point to 19
        // yPos = 2 sets the pixel point to 35
        // yPos = 3 sets the pixel point to 51
        yPixels = ((yPos - displayOffset) * 15) + 3;
        xPixels = (xPos * _font.FontWidth) + 5;
        _display->GotoXY((xPixels),(yPixels+4));
        return true;
    }

    // otherwise return false
    return false;
}

// print tools
void Menu::printPointer() {

    // dont allow printing of pointer if less than 2 items
    if (itemCnt < 2) {return;}

    // move the cursor
    // yPos = 0 sets the pixel point to 3
    // yPos = 1 sets the pixel point to 19
    // yPos = 2 sets the pixel point to 35
    // yPos = 3 sets the pixel point to 51
    uint8_t yPixels = ((pntrPos - displayOffset) * 15) + 3;
    uint8_t xPixels = 0;
    _display->GotoXY((xPixels+2),(yPixels+4));

    if (flashIsOn){ 
        _display->Print("*", _font, COLOR_WHITE);
    }
    else { 
        _display->Print(" ", _font, COLOR_WHITE);
    }
    _display->UpdateScreen();

}

void Menu::printOffsetArrows() {

    // print the offset UP arrow depending on the offset value
    // move the cursor
    // yPos = 0 sets the pixel point to 3
    // yPos = 1 sets the pixel point to 19
    // yPos = 2 sets the pixel point to 35
    // yPos = 3 sets the pixel point to 51
    uint8_t xPixels = ((DISP_CHAR_WIDTH - 1) * _font.FontWidth) + 5;
    uint8_t yPixels = (1 * 15) + 3;
    _display->GotoXY((xPixels),(yPixels+4));

    if (displayOffset > 0) {_display->Print("^", _font, COLOR_WHITE);} else {_display->Print(" ", _font, COLOR_WHITE);}

    // print the down arrow if needed
    xPixels = ((DISP_CHAR_WIDTH - 1) * _font.FontWidth) + 5;
    yPixels = (3 * 15) + 3;
    _display->GotoXY((xPixels),(yPixels+4));
    if (itemCnt > DISP_ITEM_ROWS && (itemCnt - DISP_ITEM_ROWS) > displayOffset) {_display->Print("v", _font, COLOR_WHITE);} else {_display->Print(" ", _font, COLOR_WHITE);}

    // update screen
    _display->UpdateScreen();
}

void Menu::printOnOff(bool val) {

}

void Menu::printUint32_tAtWidth(uint32_t value, uint8_t width, char c, bool isRight) {

}

