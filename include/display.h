#pragma once
/******************************************************************************
 * Project: stm32 - E4                                                        *
 * -------------------------------------------------------------------------- *
 * Copyright 2024 - James Clarke                                              *
 * This SSD1306 LCD uses I2C for communication                                *
 * Library features functions for drawing lines, rectangles and circles.      *
 * It also allows you to draw texts and characters using appropriate          *
 * functions provided in library.                                             *
 *                                                                            *
 * Default pinout                                                             *
 *                                                                            *
 * SSD1306    |STM32F411    |DESCRIPTION                                      *
 * VCC        |3.3V         |                                                 *
 * GND        |GND          |                                                 *
 * SCL        |PB8          |Serial clock line                                *
 * SDA        |PB9          |Serial data line                                 *
 * -------------------------------------------------------------------------- *
 * Licence:                                                                   *
 *     Use of this source code is governed by an MIT-style                    *
 *     license that can be found in the LICENSE file or at                    *
 *     https://opensource.org/licenses/MIT.                                   *
 ******************************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "string.h"
#include "stdlib.h"
#include "i2c.h"

/******************************************************************************
 * It is used in all LCD based libraries.
 * Supported fonts
 * Currently, these fonts are supported:
 *  - 7 x 10 pixels
 *  - 11 x 18 pixels
 *  - 16 x 26 pixels
 *  - 6 x 8 pixels
 ******************************************************************************/
/** Font structure used on my LCD libraries */
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

/** String length and height */
typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} FONTS_SIZE_t;

extern FontDef_t Font_7x10;  /** 7 x 10 pixels font size structure */
extern FontDef_t Font_11x18;  /** 11 x 18 pixels font size structure */
extern FontDef_t Font_16x26;  /** 16 x 26 pixels font size structure */
extern FontDef_t Font_6x8;  /** 16 x 26 pixels font size structure */

/** Calculates string length and height in units of pixels depending on string and font used */
//char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);

const uint16_t Font6x8 [] = {
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // sp
0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x0000, 0x2000, 0x0000,  // !
0x5000, 0x5000, 0x5000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // "
0x5000, 0x5000, 0xf800, 0x5000, 0xf800, 0x5000, 0x5000, 0x0000,  // #
0x2000, 0x7800, 0xa000, 0x7000, 0x2800, 0xf000, 0x2000, 0x0000,  // $
0xc000, 0xc800, 0x1000, 0x2000, 0x4000, 0x9800, 0x1800, 0x0000,  // %
0x4000, 0xa000, 0xa000, 0x4000, 0xa800, 0x9000, 0x6800, 0x0000,  // &
0x3000, 0x3000, 0x2000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000,  // '
0x1000, 0x2000, 0x4000, 0x4000, 0x4000, 0x2000, 0x1000, 0x0000,  // (
0x4000, 0x2000, 0x1000, 0x1000, 0x1000, 0x2000, 0x4000, 0x0000,  // )
0x2000, 0xa800, 0x7000, 0xf800, 0x7000, 0xa800, 0x2000, 0x0000,  // *
0x0000, 0x2000, 0x2000, 0xf800, 0x2000, 0x2000, 0x0000, 0x0000,  // +
0x0000, 0x0000, 0x0000, 0x0000, 0x3000, 0x3000, 0x2000, 0x0000,  // ,
0x0000, 0x0000, 0x0000, 0xf800, 0x0000, 0x0000, 0x0000, 0x0000,  // -
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3000, 0x3000, 0x0000,  // .
0x0000, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x0000, 0x0000,  // /
0x7000, 0x8800, 0x9800, 0xa800, 0xc800, 0x8800, 0x7000, 0x0000,  // 0
0x2000, 0x6000, 0x2000, 0x2000, 0x2000, 0x2000, 0x7000, 0x0000,  // 1
0x7000, 0x8800, 0x0800, 0x7000, 0x8000, 0x8000, 0xf800, 0x0000,  // 2
0xf800, 0x0800, 0x1000, 0x3000, 0x0800, 0x8800, 0x7000, 0x0000,  // 3
0x1000, 0x3000, 0x5000, 0x9000, 0xf800, 0x1000, 0x1000, 0x0000,  // 4
0xf800, 0x8000, 0xf000, 0x0800, 0x0800, 0x8800, 0x7000, 0x0000,  // 5
0x3800, 0x4000, 0x8000, 0xf000, 0x8800, 0x8800, 0x7000, 0x0000,  // 6
0xf800, 0x0800, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x0000,  // 7
0x7000, 0x8800, 0x8800, 0x7000, 0x8800, 0x8800, 0x7000, 0x0000,  // 8
0x7000, 0x8800, 0x8800, 0x7800, 0x0800, 0x1000, 0xe000, 0x0000,  // 9
0x0000, 0x0000, 0x2000, 0x0000, 0x2000, 0x0000, 0x0000, 0x0000,  // :
0x0000, 0x0000, 0x2000, 0x0000, 0x2000, 0x2000, 0x4000, 0x0000,  // ;
0x0800, 0x1000, 0x2000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0000,  // <
0x0000, 0x0000, 0xf800, 0x0000, 0xf800, 0x0000, 0x0000, 0x0000,  // =
0x4000, 0x2000, 0x1000, 0x0800, 0x1000, 0x2000, 0x4000, 0x0000,  // >
0x7000, 0x8800, 0x0800, 0x3000, 0x2000, 0x0000, 0x2000, 0x0000,  // ?
0x7000, 0x8800, 0xa800, 0xb800, 0xb000, 0x8000, 0x7800, 0x0000,  // @
0x2000, 0x5000, 0x8800, 0x8800, 0xf800, 0x8800, 0x8800, 0x0000,  // A
0xf000, 0x8800, 0x8800, 0xf000, 0x8800, 0x8800, 0xf000, 0x0000,  // B
0x7000, 0x8800, 0x8000, 0x8000, 0x8000, 0x8800, 0x7000, 0x0000,  // C
0xf000, 0x8800, 0x8800, 0x8800, 0x8800, 0x8800, 0xf000, 0x0000,  // D
0xf800, 0x8000, 0x8000, 0xf000, 0x8000, 0x8000, 0xf800, 0x0000,  // E
0xf800, 0x8000, 0x8000, 0xf000, 0x8000, 0x8000, 0x8000, 0x0000,  // F
0x7800, 0x8800, 0x8000, 0x8000, 0x9800, 0x8800, 0x7800, 0x0000,  // G
0x8800, 0x8800, 0x8800, 0xf800, 0x8800, 0x8800, 0x8800, 0x0000,  // H
0x7000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x7000, 0x0000,  // I
0x3800, 0x1000, 0x1000, 0x1000, 0x1000, 0x9000, 0x6000, 0x0000,  // J
0x8800, 0x9000, 0xa000, 0xc000, 0xa000, 0x9000, 0x8800, 0x0000,  // K
0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0xf800, 0x0000,  // L
0x8800, 0xd800, 0xa800, 0xa800, 0xa800, 0x8800, 0x8800, 0x0000,  // M
0x8800, 0x8800, 0xc800, 0xa800, 0x9800, 0x8800, 0x8800, 0x0000,  // N
0x7000, 0x8800, 0x8800, 0x8800, 0x8800, 0x8800, 0x7000, 0x0000,  // O
0xf000, 0x8800, 0x8800, 0xf000, 0x8000, 0x8000, 0x8000, 0x0000,  // P
0x7000, 0x8800, 0x8800, 0x8800, 0xa800, 0x9000, 0x6800, 0x0000,  // Q
0xf000, 0x8800, 0x8800, 0xf000, 0xa000, 0x9000, 0x8800, 0x0000,  // R
0x7000, 0x8800, 0x8000, 0x7000, 0x0800, 0x8800, 0x7000, 0x0000,  // S
0xf800, 0xa800, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x0000,  // T
0x8800, 0x8800, 0x8800, 0x8800, 0x8800, 0x8800, 0x7000, 0x0000,  // U
0x8800, 0x8800, 0x8800, 0x8800, 0x8800, 0x5000, 0x2000, 0x0000,  // V
0x8800, 0x8800, 0x8800, 0xa800, 0xa800, 0xa800, 0x5000, 0x0000,  // W
0x8800, 0x8800, 0x5000, 0x2000, 0x5000, 0x8800, 0x8800, 0x0000,  // X
0x8800, 0x8800, 0x5000, 0x2000, 0x2000, 0x2000, 0x2000, 0x0000,  // Y
0xf800, 0x0800, 0x1000, 0x7000, 0x4000, 0x8000, 0xf800, 0x0000,  // Z
0x7800, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x7800, 0x0000,  // [
0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0000, 0x0000,  /* \ */
0x7800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x7800, 0x0000,  // ]
0x2000, 0x5000, 0x8800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // ^
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf800, 0x0000,  // _
0x6000, 0x6000, 0x2000, 0x1000, 0x0000, 0x0000, 0x0000, 0x0000,  // `
0x0000, 0x0000, 0x6000, 0x1000, 0x7000, 0x9000, 0x7800, 0x0000,  // a
0x8000, 0x8000, 0xb000, 0xc800, 0x8800, 0xc800, 0xb000, 0x0000,  // b
0x0000, 0x0000, 0x7000, 0x8800, 0x8000, 0x8800, 0x7000, 0x0000,  // c
0x0800, 0x0800, 0x6800, 0x9800, 0x8800, 0x9800, 0x6800, 0x0000,  // d
0x0000, 0x0000, 0x7000, 0x8800, 0xf800, 0x8000, 0x7000, 0x0000,  // e
0x1000, 0x2800, 0x2000, 0x7000, 0x2000, 0x2000, 0x2000, 0x0000,  // f
0x0000, 0x0000, 0x7000, 0x9800, 0x9800, 0x6800, 0x0800, 0x0000,  // g
0x8000, 0x8000, 0xb000, 0xc800, 0x8800, 0x8800, 0x8800, 0x0000,  // h
0x2000, 0x0000, 0x6000, 0x2000, 0x2000, 0x2000, 0x7000, 0x0000,  // i
0x1000, 0x0000, 0x1000, 0x1000, 0x1000, 0x9000, 0x6000, 0x0000,  // j
0x8000, 0x8000, 0x9000, 0xa000, 0xc000, 0xa000, 0x9000, 0x0000,  // k
0x6000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x7000, 0x0000,  // l
0x0000, 0x0000, 0xd000, 0xa800, 0xa800, 0xa800, 0xa800, 0x0000,  // m
0x0000, 0x0000, 0xb000, 0xc800, 0x8800, 0x8800, 0x8800, 0x0000,  // n
0x0000, 0x0000, 0x7000, 0x8800, 0x8800, 0x8800, 0x7000, 0x0000,  // o
0x0000, 0x0000, 0xb000, 0xc800, 0xc800, 0xb000, 0x8000, 0x0000,  // p
0x0000, 0x0000, 0x6800, 0x9800, 0x9800, 0x6800, 0x0800, 0x0000,  // q
0x0000, 0x0000, 0xb000, 0xc800, 0x8000, 0x8000, 0x8000, 0x0000,  // r
0x0000, 0x0000, 0x7800, 0x8000, 0x7000, 0x0800, 0xf000, 0x0000,  // s
0x2000, 0x2000, 0xf800, 0x2000, 0x2000, 0x2800, 0x1000, 0x0000,  // t
0x0000, 0x0000, 0x8800, 0x8800, 0x8800, 0x9800, 0x6800, 0x0000,  // u
0x0000, 0x0000, 0x8800, 0x8800, 0x8800, 0x5000, 0x2000, 0x0000,  // v
0x0000, 0x0000, 0x8800, 0x8800, 0xa800, 0xa800, 0x5000, 0x0000,  // w
0x0000, 0x0000, 0x8800, 0x5000, 0x2000, 0x5000, 0x8800, 0x0000,  // x
0x0000, 0x0000, 0x8800, 0x8800, 0x7800, 0x0800, 0x8800, 0x0000,  // y
0x0000, 0x0000, 0xf800, 0x1000, 0x2000, 0x4000, 0xf800, 0x0000,  // z
0x1000, 0x2000, 0x2000, 0x4000, 0x2000, 0x2000, 0x1000, 0x0000,  // {
0x2000, 0x2000, 0x2000, 0x0000, 0x2000, 0x2000, 0x2000, 0x0000,  // |
0x4000, 0x2000, 0x2000, 0x1000, 0x2000, 0x2000, 0x4000, 0x0000,  // }
0x4000, 0xa800, 0x1000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  // ~
};

/* I2C address */
#ifndef DISPLAY_I2C_ADDR
#define DISPLAY_I2C_ADDR         0x3C
//#define DISPLAY_I2C_ADDR       0x7A
#endif

/* SSD1306 settings */
/* SSD1306 width in pixels */
#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH            128
#endif
/* SSD1306 LCD height in pixels */
#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT           64
#endif

typedef enum {
	COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} DISPLAY_COLOR_t;

#define DISPLAY_RIGHT_HORIZONTAL_SCROLL              0x26
#define DISPLAY_LEFT_HORIZONTAL_SCROLL               0x27
#define DISPLAY_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define DISPLAY_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define DISPLAY_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define DISPLAY_ACTIVATE_SCROLL                      0x2F // Start scroll
#define DISPLAY_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range
#define DISPLAY_NORMALDISPLAY                        0xA6
#define DISPLAY_INVERTDISPLAY                        0xA7

/* Write command */
#define WRITECOMMAND(command)      i2c_writeByte(DISPLAY_I2C_ADDR, 0x00, (command))
/* Write data */
#define WRITEDATA(data)            i2c_writeByte(DISPLAY_I2C_ADDR, 0x40, (data))
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

class Display;
extern Display display;

class Display
{
public:
    Display() : Counter(0) {}
    Display(uint8_t Value) : Counter(Value){}

    // display code from here

    uint8_t Init(void) {
        /* Init I2C */
        i2c_init();
        I2C_Init();
            
        /* A little delay */
        uint32_t p = 2500;
        while(p>0)
            p--;
        
        /* Init LCD */
        WRITECOMMAND(0xAE); //display off
        WRITECOMMAND(0x20); //Set Memory Addressing Mode   
        WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
        WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
        WRITECOMMAND(0xC8); //Set COM Output Scan Direction
        WRITECOMMAND(0x00); //---set low column address
        WRITECOMMAND(0x10); //---set high column address
        WRITECOMMAND(0x40); //--set start line address
        WRITECOMMAND(0x81); //--set contrast control register
        WRITECOMMAND(0xFF);
        WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
        WRITECOMMAND(0xA6); //--set normal display
        WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
        WRITECOMMAND(0x3F); //
        WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
        WRITECOMMAND(0xD3); //-set display offset
        WRITECOMMAND(0x00); //-not offset
        WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
        WRITECOMMAND(0xF0); //--set divide ratio
        WRITECOMMAND(0xD9); //--set pre-charge period
        WRITECOMMAND(0x22); //
        WRITECOMMAND(0xDA); //--set com pins hardware configuration
        WRITECOMMAND(0x12);
        WRITECOMMAND(0xDB); //--set vcomh
        WRITECOMMAND(0x20); //0x20,0.77xVcc
        WRITECOMMAND(0x8D); //--set DC-DC enable
        WRITECOMMAND(0x14); //
        WRITECOMMAND(0xAF); //--turn on SSD1306 panel
        WRITECOMMAND(DISPLAY_DEACTIVATE_SCROLL);

        /* Clear screen */
        Fill(COLOR_BLACK);
        
        /* Update screen */
        UpdateScreen();
        
        /* Set default values */
        SSD1306.CurrentX = 0;
        SSD1306.CurrentY = 0;
        
        /* Initialized OK */
        SSD1306.Initialized = 1;
        
        /* Return OK */
        return 1;
    }

    void InvertDisplay (int i)
    {
        if (i) 
            WRITECOMMAND (DISPLAY_INVERTDISPLAY);
        else 
            WRITECOMMAND (DISPLAY_NORMALDISPLAY);
    }

    void GotoXY(uint16_t x, uint16_t y) {
        /* Set write pointers */
        SSD1306.CurrentX = x;
        SSD1306.CurrentY = y;
    }

    void DrawPixel(uint16_t x, uint16_t y, DISPLAY_COLOR_t color) {
        if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) {
            /* Error */
            return;
        }
        
        /* Check if pixels are inverted */
        if (SSD1306.Inverted) {
            if (color == COLOR_BLACK)
                color = COLOR_WHITE;
            else
                color = COLOR_BLACK;
        }
        
        /* Set color */
        if (color == COLOR_WHITE) {
            DISPLAY_Buffer[x + (y / 8) * DISPLAY_WIDTH] |= 1 << (y % 8);
        } else {
            DISPLAY_Buffer[x + (y / 8) * DISPLAY_WIDTH] &= ~(1 << (y % 8));
        }
    }

    char Putc(char ch, FontDef_t* Font, DISPLAY_COLOR_t color) {
        uint32_t i, b, j;
        
        DISPLAY_COLOR_t notcolor;

        if (color == COLOR_BLACK)
            notcolor = COLOR_WHITE;
        else
            notcolor = COLOR_BLACK;

        /* Check available space in LCD */
        if (
            DISPLAY_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
            DISPLAY_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
        ) {
            /* Error */
            return 0;
        }
        
        /* Go through font */
        for (i = 0; i < Font->FontHeight; i++) {
            b = Font->data[(ch - 32) * Font->FontHeight + i];
            for (j = 0; j < Font->FontWidth; j++) {
                if ((b << j) & 0x8000) {
                    DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), color);
                } else {
                    DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), notcolor);
                }
            }
        }
        
        /* Increase pointer */
        SSD1306.CurrentX += Font->FontWidth;
        
        /* Return character written */
        return ch;
    }

    char Puts(char* str, FontDef_t* Font, DISPLAY_COLOR_t color) {
        /* Write characters */
        while (*str) {
            /* Write character by character */
            if (Putc(*str, Font, color) != *str) {
                /* Return error */
                return *str;
            }
            
            /* Increase string pointer */
            str++;
        }
        
        /* Everything OK, zero should be returned */
        return *str;
    }

    void UpdateScreen(void) {
        uint8_t m;
        
        for (m = 0; m < 8; m++) {
            WRITECOMMAND(0xB0 + m);
            WRITECOMMAND(0x00);
            WRITECOMMAND(0x10);
            /* Write multi data */
            I2C_WriteMultiBytes(DISPLAY_I2C_ADDR, 0x40, &DISPLAY_Buffer[DISPLAY_WIDTH * m], DISPLAY_WIDTH);
        }
    }

    void ToggleInvert(void) {
        uint16_t i;
        
        /* Toggle invert */
        SSD1306.Inverted = !SSD1306.Inverted;
        
        /* Do memory toggle */
        for (i = 0; i < sizeof(DISPLAY_Buffer); i++) {
            DISPLAY_Buffer[i] = ~DISPLAY_Buffer[i];
        }
    }

    void Fill(DISPLAY_COLOR_t color) {
        /* Set memory */
        memset(DISPLAY_Buffer, (color == COLOR_BLACK) ? 0x00 : 0xFF, sizeof(DISPLAY_Buffer));
    }

    void Clear (void)
    {
        Fill ((DISPLAY_COLOR_t)0);
        UpdateScreen();
    }

    void On(void) {
        WRITECOMMAND(0x8D);  
        WRITECOMMAND(0x14);  
        WRITECOMMAND(0xAF);  
    }

    void Off(void) {
        WRITECOMMAND(0x8D);  
        WRITECOMMAND(0x10);
        WRITECOMMAND(0xAE);  
    }

    void I2C_Init() {
        uint32_t p = 250000;
        while(p>0)
            p--;
    }

    void I2C_WriteMultiBytes(uint8_t address, uint8_t reg, char* data, uint16_t count) {
        //uint8_t dt[256];
        //iint8_t i;
        //for(i = 0; i < count; i++)
        //dt[i] = data[i];
        i2c_WriteMulti(address,reg,data,count);
    }

    void I2C_Write(uint8_t address, uint8_t reg, uint8_t data) {
        i2c_writeByte(address,reg,data);
    }

    void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, DISPLAY_COLOR_t c) {
        int16_t dx, dy, sx, sy, err, e2, i, tmp; 
        
        /* Check for overflow */
        if (x0 >= DISPLAY_WIDTH) {
            x0 = DISPLAY_WIDTH - 1;
        }
        if (x1 >= DISPLAY_WIDTH) {
            x1 = DISPLAY_WIDTH - 1;
        }
        if (y0 >= DISPLAY_HEIGHT) {
            y0 = DISPLAY_HEIGHT - 1;
        }
        if (y1 >= DISPLAY_HEIGHT) {
            y1 = DISPLAY_HEIGHT - 1;
        }
        
        dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
        dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
        sx = (x0 < x1) ? 1 : -1; 
        sy = (y0 < y1) ? 1 : -1; 
        err = ((dx > dy) ? dx : -dy) / 2; 

        if (dx == 0) {
            if (y1 < y0) {
                tmp = y1;
                y1 = y0;
                y0 = tmp;
            }
            
            if (x1 < x0) {
                tmp = x1;
                x1 = x0;
                x0 = tmp;
            }
            
            /* Vertical line */
            for (i = y0; i <= y1; i++) {
                DrawPixel(x0, i, c);
            }
            
            /* Return from function */
            return;
        }
        
        if (dy == 0) {
            if (y1 < y0) {
                tmp = y1;
                y1 = y0;
                y0 = tmp;
            }
            
            if (x1 < x0) {
                tmp = x1;
                x1 = x0;
                x0 = tmp;
            }
            
            /* Horizontal line */
            for (i = x0; i <= x1; i++) {
                DrawPixel(i, y0, c);
            }
            
            /* Return from function */
            return;
        }
        
        while (1) {
            DrawPixel(x0, y0, c);
            if (x0 == x1 && y0 == y1) {
                break;
            }
            e2 = err; 
            if (e2 > -dx) {
                err -= dy;
                x0 += sx;
            } 
            if (e2 < dy) {
                err += dx;
                y0 += sy;
            } 
        }
    }

    void DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, DISPLAY_COLOR_t c) {
        /* Check input parameters */
        if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) {
            /* Return error */
            return;
        }
        
        /* Check width and height */
        if ((x + w) >= DISPLAY_WIDTH) {
            w = DISPLAY_WIDTH - x;
        }
        if ((y + h) >= DISPLAY_HEIGHT) {
            h = DISPLAY_HEIGHT - y;
        }
        
        /* Draw 4 lines */
        DrawLine(x, y, x + w, y, c);         /* Top line */
        DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
        DrawLine(x, y, x, y + h, c);         /* Left line */
        DrawLine(x + w, y, x + w, y + h, c); /* Right line */
    }

private:

    uint8_t Counter;

    /* Private SSD1306 structure */
    typedef struct {
        uint16_t CurrentX;
        uint16_t CurrentY;
        uint8_t Inverted;
        uint8_t Initialized;
    } DISPLAY_t;

    DISPLAY_t SSD1306;
    char DISPLAY_Buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

    char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font) {
        /* Fill settings */
        SizeStruct->Height = Font->FontHeight;
        SizeStruct->Length = Font->FontWidth * strlen(str);
        /* Return pointer */
        return str;
    }

};

#ifdef __cplusplus
}
#endif

#endif // DISPLAY_H