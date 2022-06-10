#ifndef SCREEN__
#define SCREEN__

#include <stdint.h>
#include "FontLib.h"

#define rgb(r, g, b)                                                             \
    (((unsigned short)(r) << 8 & 0xf800) | ((unsigned short)(g) << 3 & 0x07e0) | \
     ((unsigned short)(b) >> 3))

enum
{
    white = rgb(255, 255, 255),
    black = rgb(0, 0, 0),
    red = rgb(255, 0, 0),
    green = rgb(0, 255, 0),
    blue = rgb(0, 0, 255),
    yellow = rgb(255, 255, 0),
    orange = rgb(255, 165, 0),
};

void ScreenInit(uint16_t color);
void ScreenClear(uint16_t color);
inline void ScreenPoint(short x, short y, uint16_t color);
inline void ScreenFillRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
inline void ScreenLine(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
void ScreenFillCircle(short x_center, short y_center, uint16_t radius, uint16_t color);
void ScreenCircle(short x_center, short y_center, uint16_t radius, uint16_t color);
void ScreenRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
void ScreenShowStr(const char* str, short ulx, short uly, uint16_t color, int backColor);
void ScreenShowChar(char ch, short ulx, short uly, uint16_t color, int backColor);
void ScreenRefreshStr(const char* newStr, short x, short y, const char* lastStr, uint16_t backColor, uint16_t color);
void ScreenSetDirection(uint8_t dir);
uint8_t ScreenGetDirection();


#endif // !

