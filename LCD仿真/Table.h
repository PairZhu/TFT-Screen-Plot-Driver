#ifndef TABLE__
#define TABLE__

#include <stdbool.h>
#include <stdint.h>
#include <easyx.h>

typedef struct
{
    // 以下数据需在初始化前设置为合适的值，初始化后可以读取，但不应修改
    uint16_t dataSize;   // 最大数据长度
    int16_t left;        // 左边界坐标
    int16_t top;         // 上边界坐标
    uint16_t height;     // 图表高度
    float yMax;          // y轴最大值
    float yMin;          // y轴最小值
    uint8_t gridLine;    // 网格线的虚线样式（为0代表无网格线，为1代表实线，正整数n代表点间隔为n的虚线）
    uint16_t gridStep;   // 网格线之间的间隔
    uint16_t xStep;      // 每个数据点的x坐标的间隔
    COLORREF axisColor;  // 坐标轴颜色
    COLORREF backColor;  // 背景颜色
    COLORREF curveColor; // 曲线颜色
    // 以下属性初始化时自动计算，不需要设置。用户可以读取，但不应修改
    uint16_t width;  // 图表宽度
    uint16_t right;  // 右边界坐标
    uint16_t bottom; // 下边界坐标
    uint16_t begin;  // 图表内循环数组的起始位置
    uint16_t end;    // 图表内循环数组的尾后位置
    float *data;     // 图标内存储的数据
    float yStep;     // y轴数据距离到像素距离的换算系数
} TableStruct;

// 初始化图表
void TableInit(TableStruct *table);
// 以某一颜色覆盖并销毁图表
void TableDelete(TableStruct *table, COLORREF color);
// 清空图表内的曲线和数据点
void TableClear(TableStruct *table);
// 自动计算并放缩图表的y轴。factor为放缩后最大值和最小值的最小单位，例如factor为10，则放缩后的y轴最大值和最小值一定为10的整数倍
void TableAutoY(TableStruct *table, float factor);
// 将图表内的数据修改为传入的数组的数据，并重新绘制图表(用于静态曲线的绘制)
void TableDraw(TableStruct *table, const float *data, uint16_t size);
// 在上一函数的基础上，自动计算放缩y轴
void TableAutoDraw(TableStruct *table, const float *data, uint16_t size, float factor);
// 在图表后添加新的数据点，并绘制。如果数据点数量超过图表的最大数据点数量，则删除最前面的数据点(用于动态曲线的绘制)
void TableUpdate(TableStruct *table, const float value);
// 在上一函数的基础上，自动计算放缩y轴
void TableAutoUpdate(TableStruct *table, const float value, float factor);
// 生成默认的图表数据结构，仅需提供必要属性参数，其他属性使用默认值，用户可自行修改
TableStruct defaultTable(uint16_t dataSize, int16_t left, int16_t top, uint16_t height, float yMax, float yMin);

#endif // !TABLE__
