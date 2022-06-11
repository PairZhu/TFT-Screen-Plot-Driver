#ifndef SCREEN__
#define SCREEN__

#include "FontLib.h"
#include <stdint.h>


// RGB888转RGB565对应的十六进制值
#define RGB(r, g, b)                                                             \
    (((unsigned short)(r) << 8 & 0xf800) | ((unsigned short)(g) << 3 & 0x07e0) | \
     ((unsigned short)(b) >> 3))

// 颜色常量
enum
{
    WHITE = RGB(255, 255, 255),
    BLACK = RGB(0, 0, 0),
    RED = RGB(255, 0, 0),
    GREEN = RGB(0, 255, 0),
    BLUE = RGB(0, 0, 255),
    YELLOW = RGB(255, 255, 0),
    ORANGE = RGB(255, 165, 0),
};
// 屏幕初始化
void ScreenInit(uint16_t color);
// 以某一颜色清屏
void ScreenClear(uint16_t color);
// 画点
inline void ScreenPoint(short x, short y, uint16_t color);
// 填充矩形
inline void ScreenFillRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
// 画线
inline void ScreenLine(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
// 填充圆
void ScreenFillCircle(short x_center, short y_center, uint16_t radius, uint16_t color);
// 画圆边框
void ScreenCircle(short x_center, short y_center, uint16_t radius, uint16_t color);
// 画矩形边框
void ScreenRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
// 显示字符串（backColor小于0即代表无背景色）
void ScreenShowStr(const char *str, short ulx, short uly, uint16_t color, int backColor);
// 显示字符（backColor小于0即代表无背景色）
void ScreenShowChar(char ch, short ulx, short uly, uint16_t color, int backColor);
// 刷新字符串（对于无背景色字符串，可以使用该函数来更新显示的文本，实现一定的优化）
void ScreenRefreshStr(const char *newStr, short x, short y, const char *lastStr, uint16_t color);
// 设置屏幕方向，参数范围0~3，各自为上一个屏幕方向旋转90度
void ScreenSetDirection(uint8_t dir);
// 获取屏幕方向
uint8_t ScreenGetDirection();

#endif
