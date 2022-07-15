#ifndef SCREEN__
#define SCREEN__

#include "FontLib.h"
#include <stdint.h>
#include <easyx.h>

// 屏幕初始化
void ScreenInit(COLORREF color);
// 以某一颜色清屏
void ScreenClear(COLORREF color);
// 画点
void ScreenPoint(short x, short y, COLORREF color);
// 填充矩形
void ScreenFillRect(short x_beg, short y_beg, short x_end, short y_end, COLORREF color);
// 画线
void ScreenLine(short x_beg, short y_beg, short x_end, short y_end, COLORREF color);
// 填充圆
void ScreenFillCircle(short x_center, short y_center, uint16_t radius, COLORREF color);
// 画圆边框
void ScreenCircle(short x_center, short y_center, uint16_t radius, COLORREF color);
// 画矩形边框
void ScreenRect(short x_beg, short y_beg, short x_end, short y_end, COLORREF color);
// 显示字符串（backColor小于0即代表无背景色）
void ScreenShowStr(const char *str, short ulx, short uly, COLORREF color, long long backColor);
// 显示字符（backColor小于0即代表无背景色）
void ScreenShowChar(char ch, short ulx, short uly, COLORREF color, long long backColor);
// 刷新字符串（对于无背景色字符串，可以使用该函数来更新显示的文本，实现一定的优化）
void ScreenRefreshStr(const char *newStr, short x, short y, const char *lastStr, COLORREF color);
// 设置屏幕方向，参数范围0~3，各自为上一个屏幕方向旋转90度
void ScreenSetDirection(uint8_t dir);
// 获取屏幕方向
uint8_t ScreenGetDirection();

#endif
