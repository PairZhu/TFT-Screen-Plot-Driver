#include "Table.h"
#include "Screen.h"
#include <stdint.h>
#include <stdlib.h>

#define swap(a, b)  \
    do              \
    {               \
        (a) ^= (b); \
        (b) ^= (a); \
        (a) ^= (b); \
    } while (0)

#define abs(x) ((x) > 0 ? (x) : -(x))

#define TablePoint(x, y, c)                              \
    do                                                   \
    {                                                    \
        if ((y) >= table->top && (y) <= table->bottom && \
            (x) >= table->left && (x) <= table->right)   \
        {                                                \
            ScreenPoint((x), (y), (c));                  \
        }                                                \
    } while (0)

#define realSize (table->dataSize + 1)

#define ForEachPoint(i, last_i, x) for (short(i) = table->begin, (last_i), (x) = table->left + 1; \
                                        (i) != table->end;                                        \
                                        (last_i) = (i), (i) = ((i) + 1) % realSize, (x) += table->xStep)

#define getY(value) (table->bottom - table->yStep * ((value)-table->yMin))
#define getX(index) (table->left + 1 + (index)*table->xStep)

static void dotted_line_h(const TableStruct *table, short x_beg, short x_end, short y);
static void dotted_line_v(const TableStruct *table, short y_beg, short y_end, short x);
static void clearLine(const TableStruct *table, short x_beg, short y_beg, short x_end, short y_end);
static void clearPoint(const TableStruct *table, short x, short y);
static void TableLine(const TableStruct *table, short x_beg, short y_beg, short x_end, short y_end);
static void clearCurve(const TableStruct *table);
static void drawCurve(TableStruct *table);

void dotted_line_h(const TableStruct *table, short x_beg, short x_end, short y)
{
    if (x_beg > x_end)
        swap(x_beg, x_end);
    for (short x = x_beg; x <= x_end; ++x)
    {
        if (x % table->gridLine == 0)
            ScreenPoint(x, y, table->axisColor);
    }
}

void dotted_line_v(const TableStruct *table, short y_beg, short y_end, short x)
{
    if (y_beg > y_end)
        swap(y_beg, y_end);
    for (short y = y_beg; y <= y_end; ++y)
    {
        if (y % table->gridLine == 0)
            ScreenPoint(x, y, table->axisColor);
    }
}

TableStruct defaultTable(uint16_t dataSize, int16_t left, int16_t top, uint16_t height, float yMax, float yMin)
{
    TableStruct result;
    result.dataSize = dataSize;
    result.left = left;
    result.top = top;
    result.height = height;
    result.yMax = yMax;
    result.yMin = yMin;
    result.gridLine = 3;
    result.gridStep = 15;
    result.xStep = 2;
    result.axisColor = WHITE;
    result.backColor = BLACK;
    result.curveColor = RED;
    return result;
}

void TableInit(TableStruct *table)
{
    table->yStep = (float)table->height / (table->yMax - table->yMin);
    table->width = (table->dataSize - 1) * table->xStep + 1;
    table->right = table->left + table->width;
    table->bottom = table->top + table->height;
    table->begin = 0;
    table->end = 0;
    table->data = (float *)malloc(sizeof(float) * realSize);

    ScreenLine(table->left, table->bottom, table->right, table->bottom, table->axisColor);
    ScreenLine(table->left, table->top, table->left, table->bottom - 1, table->axisColor);
    ScreenFillRect(table->left + 1, table->top, table->right, table->bottom - 1, table->backColor);
    if (table->gridLine)
    {
        for (short y = table->bottom - table->gridStep; y >= table->top; y -= table->gridStep)
        {
            dotted_line_h(table, table->left + 1, table->right, y);
        }
        for (short x = table->gridStep + table->left; x <= table->right; x += table->gridStep)
        {
            dotted_line_v(table, table->top, table->bottom - 1, x);
        }
    }
}

void TableDelete(TableStruct *table, COLORREF color)
{
    table->begin = 0;
    table->end = 0;
    free(table->data);
    ScreenFillRect(table->left, table->top, table->right, table->bottom, color);
}

void clearPoint(const TableStruct *table, short x, short y)
{
    if (y == table->bottom)
    {
        ScreenPoint(x, y, table->axisColor);
        return;
    }
    if (table->gridLine && (((x % table->gridLine == 0) && (y - table->bottom) % table->gridStep == 0) ||
                            ((y % table->gridLine == 0) && (x - table->left) % table->gridStep == 0)))
        TablePoint(x, y, table->axisColor);
    else
        TablePoint(x, y, table->backColor);
}

void clearLine(const TableStruct *table, short x_beg, short y_beg, short x_end, short y_end)
{
    if (y_beg == y_end && y_beg == table->bottom)
    {
        ScreenLine(x_beg, y_beg, x_end, y_end, table->axisColor);
        return;
    }
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
        clearPoint(table, x, y);
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
            clearPoint(table, x, y);
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
        clearPoint(table, x, y);
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
            clearPoint(table, x, y);
        }
    }
}

void TableAutoY(TableStruct *table, float factor)
{
    clearCurve(table);
    float max = table->data[table->begin], min = max;
    for (uint16_t i = table->begin; (i + 1) % realSize != table->end; i = (i + 1) % realSize)
    {
        if (table->data[i] > max)
            max = table->data[i];
        if (table->data[i] < min)
            min = table->data[i];
    }
    if (factor > 0)
    {
        max = factor * ((int)(max / factor) + 1);
        min = factor * ((int)(min / factor) - 1);
    }
    table->yMax = max;
    table->yMin = min;
    table->yStep = (float)table->height / (table->yMax - table->yMin);
    drawCurve(table);
}

void TableLine(const TableStruct *table, short x_beg, short y_beg, short x_end, short y_end)
{
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
        TablePoint(x, y, table->curveColor);
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
            TablePoint(x, y, table->curveColor);
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
        TablePoint(x, y, table->curveColor);
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
            TablePoint(x, y, table->curveColor);
        }
    }
}

void clearCurve(const TableStruct *table)
{
    ForEachPoint(i, last_i, x)
    {
        if (x == table->left + 1)
        {
            clearPoint(table, x, getY(table->data[i]));
            continue;
        }
        clearLine(table, x - table->xStep, getY(table->data[last_i]), x, getY(table->data[i]));
    }
}

void TableClear(TableStruct *table)
{
    clearCurve(table);
    table->begin = 0;
    table->end = 0;
}

void TableDraw(TableStruct *table, const float *data, uint16_t size)
{
    TableClear(table);
    table->end = size;
    for (uint16_t i = 0; i!= size; ++i)
        table->data[i] = data[i];
    drawCurve(table);
}

void TableAutoDraw(TableStruct *table, const float *data, uint16_t size, float factor)
{
    TableClear(table);
    table->end = size;
    float max = data[0], min = max;
    for (uint16_t i = 0; i!= size; ++i)
    {
        table->data[i] = data[i];
        if (data[i] > max)
            max = data[i];
        if (data[i] < min)
            min = data[i];
    }
    if (factor > 0)
    {
        max = factor * ((int)(max / factor) + 1);
        min = factor * ((int)(min / factor) - 1);
    }
    table->yMax = max;
    table->yMin = min;
    table->yStep = (float)table->height / (table->yMax - table->yMin);
    drawCurve(table);
}

void drawCurve(TableStruct *table)
{
    ForEachPoint(i, last_i, x)
    {
        if (x == table->left + 1)
        {
            TablePoint(x, getY(table->data[i]), table->curveColor);
            continue;
        }
        TableLine(table, x - table->xStep, getY(table->data[last_i]), x, getY(table->data[i]));
    }
}

void TableUpdate(TableStruct *table, const float value)
{
    if ((table->end + 1) % realSize == table->begin)
    {
        uint16_t next_i;
        ForEachPoint(i, last_i, x)
        {
            next_i = (i + 1) % realSize;
            if (x == table->left + 1)
            {
                clearPoint(table, x, getY(table->data[i]));
                if (next_i != table->end)
                    TablePoint(x, getY(table->data[next_i]), table->curveColor);
                continue;
            }
            clearLine(table, x - table->xStep, getY(table->data[last_i]), x, getY(table->data[i]));
            if (next_i != table->end)
                TableLine(table, x - table->xStep, getY(table->data[i]), x, getY(table->data[next_i]));
            else
                TableLine(table, x - table->xStep, getY(table->data[i]), x, getY(value));
        }
    }
    else if (table->begin == table->end)
    {
        TablePoint(table->left + 1, getY(value), table->curveColor);
    }
    else
    {
        uint16_t size = (table->end + realSize - table->begin) % realSize;
        uint16_t pos = (table->begin + size - 1) % realSize;
        TableLine(table, getX(size-1), getY(table->data[pos]), getX(size), getY(value));
    }
    table->data[table->end] = value;
    table->end = (table->end + 1) % realSize;
    if (table->end == table->begin)
    {
        table->begin = (table->begin + 1) % realSize;
    }
}

void TableAutoUpdate(TableStruct *table, const float value, float factor)
{
    float max = value, min = max;
    for (uint16_t i = table->begin; (i + 1) % realSize != table->end; i = (i + 1) % realSize)
    {
        if (table->data[i] > max)
            max = table->data[i];
        if (table->data[i] < min)
            min = table->data[i];
    }
    if (factor > 0)
    {
        max = factor * ((int)(max / factor) + 1);
        min = factor * ((int)(min / factor) - 1);
    }
    if(abs(max-table->yMax) <= 1e-6 && abs(min-table->yMin) <= 1e-6)
    {
        TableUpdate(table, value);
        return;
    }
    clearCurve(table);
    table->data[table->end] = value;
    table->end = (table->end + 1) % realSize;
    if (table->end == table->begin)
    {
        table->begin = (table->begin + 1) % realSize;
    }
    table->yMax = max;
    table->yMin = min;
    table->yStep = (float)table->height / (table->yMax - table->yMin);
    drawCurve(table);
}
