#ifndef TABLE__
#define TABLE__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t dataSize;
    int16_t left;
    int16_t top;
    uint16_t height;
    float yMax;
    float yMin;
    uint8_t gridLine;
    uint16_t gridStep;
    uint16_t xStep;
    uint16_t axisColor;
    uint16_t backColor;
    uint16_t curveColor;
    // 以下为计算属性，不需要设置
    uint16_t width;
    uint16_t right;
    uint16_t bottom;
    uint16_t begin;
    uint16_t end;
    float *data;
    float yStep;
} TableStruct;

void TableInit(TableStruct *table);
void TableDelete(TableStruct *table, uint16_t color);
void TableClear(TableStruct *table);
void TableDraw(TableStruct *table, const float *data, uint16_t size);
void TableAutoDraw(TableStruct *table, const float *data, uint16_t size, float factor);
void TableAutoY(TableStruct *table, float factor);
void TableUpdate(TableStruct *table, const float value);
void TableAutoUpdate(TableStruct *table, const float value, float factor);
TableStruct defaultTable(uint16_t dataSize,int16_t left, int16_t top, uint16_t height, float yMax, float yMin);

#endif // !TABLE__
