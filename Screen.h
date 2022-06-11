#ifndef SCREEN__
#define SCREEN__

#include "FontLib.h"
#include <stdint.h>


// RGB888תRGB565��Ӧ��ʮ������ֵ
#define RGB(r, g, b)                                                             \
    (((unsigned short)(r) << 8 & 0xf800) | ((unsigned short)(g) << 3 & 0x07e0) | \
     ((unsigned short)(b) >> 3))

// ��ɫ����
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
// ��Ļ��ʼ��
void ScreenInit(uint16_t color);
// ��ĳһ��ɫ����
void ScreenClear(uint16_t color);
// ����
inline void ScreenPoint(short x, short y, uint16_t color);
// ������
inline void ScreenFillRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
// ����
inline void ScreenLine(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
// ���Բ
void ScreenFillCircle(short x_center, short y_center, uint16_t radius, uint16_t color);
// ��Բ�߿�
void ScreenCircle(short x_center, short y_center, uint16_t radius, uint16_t color);
// �����α߿�
void ScreenRect(short x_beg, short y_beg, short x_end, short y_end, uint16_t color);
// ��ʾ�ַ�����backColorС��0�������ޱ���ɫ��
void ScreenShowStr(const char *str, short ulx, short uly, uint16_t color, int backColor);
// ��ʾ�ַ���backColorС��0�������ޱ���ɫ��
void ScreenShowChar(char ch, short ulx, short uly, uint16_t color, int backColor);
// ˢ���ַ����������ޱ���ɫ�ַ���������ʹ�øú�����������ʾ���ı���ʵ��һ�����Ż���
void ScreenRefreshStr(const char *newStr, short x, short y, const char *lastStr, uint16_t color);
// ������Ļ���򣬲�����Χ0~3������Ϊ��һ����Ļ������ת90��
void ScreenSetDirection(uint8_t dir);
// ��ȡ��Ļ����
uint8_t ScreenGetDirection();

#endif
