#include <string.h>
#include "LCD.h"

#include "Screen.h"

static uint8_t direction = 0;

#define swap(a, b)  \
    do              \
    {               \
        (a) ^= (b); \
        (b) ^= (a); \
        (a) ^= (b); \
    } while (0)

#define coordTrans(x, y)            \
    do                              \
    {                               \
        short temp;                 \
        switch (direction)          \
        {                           \
        case 0:                     \
            break;                  \
        case 1:                     \
            temp = (x);             \
            (x) = LCD_WIDTH - 1 - (y);  \
            (y) = temp;             \
            break;                  \
        case 2:                     \
            (x) = LCD_WIDTH - 1 - (x);  \
            (y) = LCD_LENGTH - 1 - (y); \
            break;                  \
        case 3:                     \
            temp = (y);             \
            (y) = LCD_LENGTH - 1 - (x); \
            (x) = temp;             \
            break;                  \
        default:                    \
            break;                  \
        }                           \
    } while (0)

void ScreenSetDirection(uint8_t dir)
{
    direction = dir;
}

uint8_t ScreenGetDirection()
{
    return direction;
}

inline void ScreenPoint(short x, short y, uint16_t color)
{
    coordTrans(x, y);
    if (x >= 0 && y >= 0 && x < LCD_WIDTH && y < LCD_LENGTH)
        point(x ,y , color);
}

inline void ScreenFillRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color)
{
    coordTrans(x_beg, y_beg);
    coordTrans(x_end, y_end);
    if (x_beg > x_end)
        swap(x_beg, x_end);
    if (y_beg > y_end)
        swap(y_beg, y_end);

    // 如果最小值大于屏幕的最大坐标，或者最大值小于屏幕的最小坐标，则不用绘制
    if (x_beg >= LCD_WIDTH || y_beg >= LCD_LENGTH || x_end < 0 || y_end < 0)
        return;

    // 如果最小值小于屏幕的最小坐标，则将最小值设置为屏幕的最小坐标,同理最大值也一样
    if (x_beg < 0)
        x_beg = 0;
    if (y_beg < 0)
        y_beg = 0;
    if (x_end >= LCD_WIDTH)
        x_end = LCD_WIDTH - 1;
    if (y_end >= LCD_LENGTH)
        y_end = LCD_LENGTH - 1;

    fillRect(x_beg, y_beg, x_end, y_end, color);
}

void ScreenClear(uint16_t color)
{
    fillRect(0, 0, LCD_WIDTH - 1, LCD_LENGTH - 1, color);
}

void ScreenShowChar(char ch, short ulx, short uly, uint16_t color, int backColor)
{
    if (ch == ' ' && backColor < 0)
        return;
    const unsigned char *font = getFont(ch);
    short y = uly;
    for (uint8_t pos = 0; pos < fontHeight; ++pos)
    {
        short x = ulx;
        for (uint8_t i = 1; i != 1 << 6; i <<= 1, ++x)
        {
            if (font[pos] & i)
            {
                ScreenPoint(x, y, color);
            }
            else if (backColor >= 0)
            {
                ScreenPoint(x, y, backColor);
            }
        }
        ++y;
    }
}

void ScreenShowStr(const char *str, short ulx, short uly, uint16_t color, int backColor)
{
    short x = ulx;
    while (*str != '\0')
    {
        ScreenShowChar(*str++, x, uly, color, backColor);
        x += fontWidth;
    }
}

void ScreenRefreshStr(const char *newStr, short x, short y, const char *lastStr, uint16_t backColor, uint16_t color)
{
    int newStrLen = strlen(newStr), lastStrLen = strlen(lastStr);
    int maxLength = newStrLen > lastStrLen ? newStrLen : lastStrLen;
    for (int i = 0; i != maxLength; ++i, x += fontWidth)
    {
        if (i >= newStrLen)
        {
            ScreenShowChar(' ', x, y, color, backColor);
        }
        else if (i >= lastStrLen || newStr[i] != lastStr[i])
        {
            ScreenShowChar(newStr[i], x, y, color, backColor);
        }
    }
}

void ScreenRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color)
{
    ScreenLine(x_beg, y_beg, x_end, y_beg, color);
    ScreenLine(x_beg, y_end, x_end, y_end, color);
    ScreenLine(x_beg, y_beg, x_beg, y_end, color);
    ScreenLine(x_end, y_beg, x_end, y_end, color);
}

void ScreenCircle(short x_center, short y_center, uint16_t radius, uint16_t color)
{
    short x = 0, y = radius, d = 3 - (radius << 1);
    while (x <= y)
    {
        ScreenPoint(x_center + x, y_center + y, color);
        ScreenPoint(x_center - x, y_center + y, color);
        ScreenPoint(x_center + x, y_center - y, color);
        ScreenPoint(x_center - x, y_center - y, color);
        ScreenPoint(x_center + y, y_center + x, color);
        ScreenPoint(x_center - y, y_center + x, color);
        ScreenPoint(x_center + y, y_center - x, color);
        ScreenPoint(x_center - y, y_center - x, color);
        if (d < 0)
        {
            d += (x << 2) + 6;
        }
        else
        {
            d += ((x - y) << 2) + 10;
            y--;
        }
        x++;
    }
}

void ScreenFillCircle(short x_center, short y_center, uint16_t radius, uint16_t color)
{
    short x = 0, y = radius, d = 3 - (radius << 1);
    while (x <= y)
    {
        ScreenLine(x_center + x, y_center - y, x_center + x, y_center + y, color);
        ScreenLine(x_center - x, y_center - y, x_center - x, y_center + y, color);
        ScreenLine(x_center + y, y_center - x, x_center + y, y_center + x, color);
        ScreenLine(x_center - y, y_center - x, x_center - y, y_center + x, color);
        if (d < 0)
        {
            d += (x << 2) + 6;
        }
        else
        {
            d += ((x - y) << 2) + 10;
            y--;
        }
        x++;
    }
}

inline void ScreenLine(short x_beg, short y_beg, short x_end, short y_end, uint16_t color)
{
    if (x_beg == x_end || y_beg == y_end)
        ScreenFillRect(x_beg, y_beg, x_end, y_end, color);
    short x, y;
    short dx = x_end - x_beg, dy = y_end - y_beg;
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    if (dx > dy)
    {
        short twod = dy << 1, twodsubdx = (dy - dx) << 1;
        short p = twod - dx;
        if (x_beg > x_end)
        {
            x = x_end;
            y = y_end;
            x_end = x_beg;
            y_end = y_beg;
        }
        else
        {
            x = x_beg;
            y = y_beg;
        }
        ScreenPoint(x, y, color);
        while (x < x_end)
        {
            ++x;
            if (p < 0)
            {
                p += twod;
            }
            else
            {
                if (y_beg < y_end)
                    ++y;
                else
                    --y;
                p += twodsubdx;
            }
            ScreenPoint(x, y, color);
        }
    }
    else
    {
        short twod = dx << 1, twodsubdx = (dx - dy) << 1;
        short p = twod - dy;
        if (y_beg > y_end)
        {
            x = x_end;
            y = y_end;
            x_end = x_beg;
            y_end = y_beg;
        }
        else
        {
            x = x_beg;
            y = y_beg;
        }
        ScreenPoint(x, y, color);
        while (y < y_end)
        {
            ++y;
            if (p < 0)
            {
                p += twod;
            }
            else
            {
                if (x_beg < x_end)
                    ++x;
                else
                    --x;
                p += twodsubdx;
            }
            ScreenPoint(x, y, color);
        }
    }
}

void ScreenInit(uint16_t color)
{
    lcdInit();
    ScreenClear(color);
    setBackLight(true);
}
