#ifndef TABLE__
#define TABLE__

#include <stdbool.h>
#include <stdint.h>
#include <easyx.h>

typedef struct
{
    // �����������ڳ�ʼ��ǰ����Ϊ���ʵ�ֵ����ʼ������Զ�ȡ������Ӧ�޸�
    uint16_t dataSize;   // ������ݳ���
    int16_t left;        // ��߽�����
    int16_t top;         // �ϱ߽�����
    uint16_t height;     // ͼ��߶�
    float yMax;          // y�����ֵ
    float yMin;          // y����Сֵ
    uint8_t gridLine;    // �����ߵ�������ʽ��Ϊ0�����������ߣ�Ϊ1����ʵ�ߣ�������n�������Ϊn�����ߣ�
    uint16_t gridStep;   // ������֮��ļ��
    uint16_t xStep;      // ÿ�����ݵ��x����ļ��
    COLORREF axisColor;  // ��������ɫ
    COLORREF backColor;  // ������ɫ
    COLORREF curveColor; // ������ɫ
    // �������Գ�ʼ��ʱ�Զ����㣬����Ҫ���á��û����Զ�ȡ������Ӧ�޸�
    uint16_t width;  // ͼ����
    uint16_t right;  // �ұ߽�����
    uint16_t bottom; // �±߽�����
    uint16_t begin;  // ͼ����ѭ���������ʼλ��
    uint16_t end;    // ͼ����ѭ�������β��λ��
    float *data;     // ͼ���ڴ洢������
    float yStep;     // y�����ݾ��뵽���ؾ���Ļ���ϵ��
} TableStruct;

// ��ʼ��ͼ��
void TableInit(TableStruct *table);
// ��ĳһ��ɫ���ǲ�����ͼ��
void TableDelete(TableStruct *table, COLORREF color);
// ���ͼ���ڵ����ߺ����ݵ�
void TableClear(TableStruct *table);
// �Զ����㲢����ͼ���y�ᡣfactorΪ���������ֵ����Сֵ����С��λ������factorΪ10����������y�����ֵ����Сֵһ��Ϊ10��������
void TableAutoY(TableStruct *table, float factor);
// ��ͼ���ڵ������޸�Ϊ�������������ݣ������»���ͼ��(���ھ�̬���ߵĻ���)
void TableDraw(TableStruct *table, const float *data, uint16_t size);
// ����һ�����Ļ����ϣ��Զ��������y��
void TableAutoDraw(TableStruct *table, const float *data, uint16_t size, float factor);
// ��ͼ�������µ����ݵ㣬�����ơ�������ݵ���������ͼ���������ݵ���������ɾ����ǰ������ݵ�(���ڶ�̬���ߵĻ���)
void TableUpdate(TableStruct *table, const float value);
// ����һ�����Ļ����ϣ��Զ��������y��
void TableAutoUpdate(TableStruct *table, const float value, float factor);
// ����Ĭ�ϵ�ͼ�����ݽṹ�������ṩ��Ҫ���Բ�������������ʹ��Ĭ��ֵ���û��������޸�
TableStruct defaultTable(uint16_t dataSize, int16_t left, int16_t top, uint16_t height, float yMax, float yMin);

#endif // !TABLE__
