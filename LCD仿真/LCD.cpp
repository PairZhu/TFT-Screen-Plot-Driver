#include <stdint.h>
#include "LCD.h"
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>


void mypoint(uint16_t x, uint16_t y, COLORREF color)
{
    putpixel(x, y, color);
}

void setBackLight(bool light)
{
    
}

void fillRect(uint16_t x_beg, uint16_t y_beg, uint16_t x_end, uint16_t y_end, COLORREF color)
{
    setfillcolor(color);
    solidrectangle(x_beg, y_beg, x_end, y_end);
}

void lcdInit(void)
{
    initgraph(LCD_WIDTH, LCD_HEIGHT);
}
