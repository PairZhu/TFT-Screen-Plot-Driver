#ifndef SCREEN__
#define SCREEN__

#include "FontLib.h"
#include <stdint.h>
#include <easyx.h>

// ��Ļ��ʼ��
void ScreenInit(COLORREF color);
// ��ĳһ��ɫ����
void ScreenClear(COLORREF color);
// ����
void ScreenPoint(short x, short y, COLORREF color);
// ������
void ScreenFillRect(short x_beg, short y_beg, short x_end, short y_end, COLORREF color);
// ����
void ScreenLine(short x_beg, short y_beg, short x_end, short y_end, COLORREF color);
// ���Բ
void ScreenFillCircle(short x_center, short y_center, uint16_t radius, COLORREF color);
// ��Բ�߿�
void ScreenCircle(short x_center, short y_center, uint16_t radius, COLORREF color);
// �����α߿�
void ScreenRect(short x_beg, short y_beg, short x_end, short y_end, COLORREF color);
// ��ʾ�ַ�����backColorС��0�������ޱ���ɫ��
void ScreenShowStr(const char *str, short ulx, short uly, COLORREF color, long long backColor);
// ��ʾ�ַ���backColorС��0�������ޱ���ɫ��
void ScreenShowChar(char ch, short ulx, short uly, COLORREF color, long long backColor);
// ˢ���ַ����������ޱ���ɫ�ַ���������ʹ�øú�����������ʾ���ı���ʵ��һ�����Ż���
void ScreenRefreshStr(const char *newStr, short x, short y, const char *lastStr, COLORREF color);
// ������Ļ���򣬲�����Χ0~3������Ϊ��һ����Ļ������ת90��
void ScreenSetDirection(uint8_t dir);
// ��ȡ��Ļ����
uint8_t ScreenGetDirection();

#endif
