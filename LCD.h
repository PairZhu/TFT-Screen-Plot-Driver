#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#include <stdbool.h>

#define LCD_WIDTH 240
#define LCD_LENGTH 320

inline void point(uint16_t x, uint16_t y, uint16_t color);
inline void setBackLight(bool light);
inline void fillRect(uint16_t x_beg, uint16_t y_beg, uint16_t x_end, uint16_t y_end, uint16_t color);
inline void showImage(uint16_t x_beg, uint16_t y_beg, uint16_t x_end, uint16_t y_end, uint16_t* color);
void lcdInit(void);


#endif // __LCD_H__
