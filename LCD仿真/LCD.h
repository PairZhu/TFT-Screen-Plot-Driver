#ifndef __LCD_H__
#define __LCD_H__

#include <stdbool.h>
#include <stdint.h>
#include <easyx.h>

#define LCD_WIDTH 240  // LCD的像素宽度
#define LCD_HEIGHT 320 // LCD的像素高度

void mypoint(uint16_t x, uint16_t y, COLORREF color);
void setBackLight(bool light);
void fillRect(uint16_t x_beg, uint16_t y_beg, uint16_t x_end, uint16_t y_end, COLORREF color);
void lcdInit(void);

#endif // __LCD_H__
