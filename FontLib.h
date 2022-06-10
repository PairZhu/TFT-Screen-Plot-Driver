#ifndef __FONT_LIB_H__
#define __FONT_LIB_H__
#define fontWidth 6
#define fontHeight 12
extern const unsigned char FONTARR[][12];
const unsigned char* getFont(const char ch);
#endif
