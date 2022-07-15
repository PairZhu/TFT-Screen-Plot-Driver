#ifndef __FONT_LIB_H__
#define __FONT_LIB_H__
#define fontWidth 6                       // 字体宽度
#define fontHeight 12                     // 字体高度
extern const unsigned char FONTARR[][12]; // 字库像素数据数组
const unsigned char *getFont(char ch);    // 字符到字库像素数据的映射函数
#endif
