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
 *****************************************************************************/
#include <display.h>

/******************************************************************************
 * Constructors for class.                                                    *
 * -------------------------------------------------------------------------- *
 * @param sdapin        // pin assignment for I2C SDA                         *
 * @param sclpin        // pin assignment for I2C SCL                         *
 * @param module        // I2C module in STM32F411 (1 or 2)                   *
 *****************************************************************************/
Display::Display(uint16_t sdapin, uint16_t sclpin, uint16_t module){
    _sdapin = sdapin;
    _sclpin = sclpin;
    _module = module;
    _i2c = I2C(sdapin, sclpin, module);
}

/******************************************************************************
 * Public Methods / Function Declarations                                     *
 *****************************************************************************/
uint8_t Display::Init(void) {
    /* Init I2C */
    _i2c.i2c_init();
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

void Display::InvertDisplay (int i)
{
    if (i) 
        WRITECOMMAND (DISPLAY_INVERTDISPLAY);
    else 
        WRITECOMMAND (DISPLAY_NORMALDISPLAY);
}

void Display::GotoXY(uint16_t x, uint16_t y) {
    /* Set write pointers */
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

void Display::DrawPixel(uint16_t x, uint16_t y, DISPLAY_COLOR_t color) {
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

char Display::Putc(char ch, FontDef_t Font, DISPLAY_COLOR_t color) {
    uint32_t i, b, j;
    
    DISPLAY_COLOR_t notcolor;

    if (color == COLOR_BLACK)
        notcolor = COLOR_WHITE;
    else
        notcolor = COLOR_BLACK;

    /* Check available space in LCD */
    if (
        DISPLAY_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
        DISPLAY_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight)
    ) {
        /* Error */
        return 0;
    }
    
    /* Go through font */
    for (i = 0; i < Font.FontHeight; i++) {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for (j = 0; j < Font.FontWidth; j++) {
            if ((b << j) & 0x8000) {
                DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), color);
            } else {
                DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), notcolor);
            }
        }
    }
    
    /* Increase pointer */
    SSD1306.CurrentX += Font.FontWidth;
    
    /* Return character written */
    return ch;
}

char Display::Print(const char str[], FontDef_t Font, DISPLAY_COLOR_t color) {
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

void Display::UpdateScreen(void) {
    uint8_t m;
    
    for (m = 0; m < 8; m++) {
        WRITECOMMAND(0xB0 + m);
        WRITECOMMAND(0x00);
        WRITECOMMAND(0x10);
        /* Write multi data */
        I2C_WriteMultiBytes(DISPLAY_I2C_ADDR, 0x40, &DISPLAY_Buffer[DISPLAY_WIDTH * m], DISPLAY_WIDTH);
    }
}

void Display::ToggleInvert(void) {
    uint16_t i;
    
    /* Toggle invert */
    SSD1306.Inverted = !SSD1306.Inverted;
    
    /* Do memory toggle */
    for (i = 0; i < sizeof(DISPLAY_Buffer); i++) {
        DISPLAY_Buffer[i] = ~DISPLAY_Buffer[i];
    }
}

void Display::Fill(DISPLAY_COLOR_t color) {
    /* Set memory */
    memset(DISPLAY_Buffer, (color == COLOR_BLACK) ? 0x00 : 0xFF, sizeof(DISPLAY_Buffer));
}

void Display::Clear (void)
{
    Fill ((DISPLAY_COLOR_t)0);
    UpdateScreen();
}

void Display::On(void) {
    WRITECOMMAND(0x8D);  
    WRITECOMMAND(0x14);  
    WRITECOMMAND(0xAF);  
}

void Display::Off(void) {
    WRITECOMMAND(0x8D);  
    WRITECOMMAND(0x10);
    WRITECOMMAND(0xAE);  
}

void Display::I2C_Init() {
    uint32_t p = 250000;
    while(p>0)
        p--;
}

void Display::I2C_WriteMultiBytes(uint8_t address, uint8_t reg, char* data, uint16_t count) {
    //uint8_t dt[256];
    //iint8_t i;
    //for(i = 0; i < count; i++)
    //dt[i] = data[i];
    _i2c.i2c_WriteMulti(address,reg,data,count);
}

void Display::I2C_Write(uint8_t address, uint8_t reg, uint8_t data) {
    _i2c.i2c_writeByte(address,reg,data);
}

void Display::DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, DISPLAY_COLOR_t c) {
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

void Display::DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, DISPLAY_COLOR_t c) {
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

/******************************************************************************
 * Private Methods / Function Declarations                                    *
 ******************************************************************************/
char* Display::FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font) {
    /* Fill settings */
    SizeStruct->Height = Font->FontHeight;
    SizeStruct->Length = Font->FontWidth * strlen(str);
    /* Return pointer */
    return str;
}

void Display::WRITECOMMAND(int command) {
    _i2c.i2c_writeByte(DISPLAY_I2C_ADDR, 0x00, (command));
}

void Display::WRITEDATA(int data) {
    _i2c.i2c_writeByte(DISPLAY_I2C_ADDR, 0x40, (data));
}


