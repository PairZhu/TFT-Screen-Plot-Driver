#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include "delay.h"
#include "LCD.h"


#define CS_H MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0)
#define RS_H MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1)
#define BL_H MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2)
#define RST_H MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3)
#define WR_H MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN4)

#define CS_L MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0)
#define RS_L MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1)
#define BL_L MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2)
#define RST_L MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3)
#define WR_L MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4)

#define POUT(val)             \
    do                        \
    {                         \
        P7->OUT = (val);      \
        P9->OUT = (val) >> 8; \
    } while (0)

inline static void writeCmd(uint8_t cmd);
inline static void writeData(uint8_t data);
inline static void writeColor(uint16_t color);

inline void writeCmd(uint8_t cmd)
{
    CS_L;
    RS_L;
#ifdef SPI_LCD
    EUSCI_B0->TXBUF=cmd;
    //MAP_SPI_transmitData(EUSCI_B0_BASE, cmd);
#else
    POUT(cmd);
    WR_L;
    WR_H;
#endif
    CS_H;
}

inline void writeData(uint8_t data)
{
    CS_L;
    RS_H;
#ifdef SPI_LCD
    EUSCI_B0->TXBUF=data;
    //MAP_SPI_transmitData(EUSCI_B0_BASE, data);
#else
    POUT(data);
    WR_L;
    WR_H;
#endif
    CS_H;
}

inline void writeColor(uint16_t color)
{
    CS_L;
    RS_H;
#ifdef SPI_LCD
    EUSCI_B0->TXBUF=color>>8;
    EUSCI_B0->TXBUF=color;
    //MAP_SPI_transmitData(EUSCI_B0_BASE, color >> 8);
    //MAP_SPI_transmitData(EUSCI_B0_BASE, color);
#else
    POUT(color);
    WR_L;
    WR_H;
#endif
    CS_H;
}

inline void point(uint16_t x, uint16_t y, uint16_t color)
{
    writeCmd(0x2a);
    writeData(x >> 8);
    writeData(x);
    writeCmd(0x2b);
    writeData(y >> 8);
    writeData(y);
    writeCmd(0x2c);
    writeColor(color);
}

inline void setBackLight(bool light)
{
    if (light)
        BL_H;
    else
        BL_L;
}

inline void fillRect(uint16_t x_beg, uint16_t y_beg, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    writeCmd(0x2a); //列地址设置
    writeData(x_beg >> 8);
    writeData(x_beg);
    writeData(x_end >> 8);
    writeData(x_end);
    writeCmd(0x2b); //行地址设置
    writeData(y_beg >> 8);
    writeData(y_beg);
    writeData(y_end >> 8);
    writeData(y_end);
    writeCmd(0x2c); //储存器写
    for (uint32_t i = 0; i != (x_end - x_beg + 1) * (y_end - y_beg + 1); ++i)
    {
        writeColor(color);
    }
    writeCmd(0x2a); //列地址设置
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
    writeCmd(0x2b); //行地址设置
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
}

inline void showImage(uint16_t x_beg, uint16_t y_beg, uint16_t x_end, uint16_t y_end, uint16_t* color)
{
    writeCmd(0x2a); //列地址设置
    writeData(x_beg >> 8);
    writeData(x_beg);
    writeData(x_end >> 8);
    writeData(x_end);
    writeCmd(0x2b); //行地址设置
    writeData(y_beg >> 8);
    writeData(y_beg);
    writeData(y_end >> 8);
    writeData(y_end);
    writeCmd(0x2c); //储存器写
    for (uint32_t i = 0; i != (x_end - x_beg + 1) * (y_end - y_beg + 1); ++i)
    {
        writeColor(color[i]);
    }
    writeCmd(0x2a); //列地址设置
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
    writeCmd(0x2b); //行地址设置
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
    writeData(0xff);
}

void lcdInit(void)
{
#ifdef SPI_LCD
    const eUSCI_SPI_MasterConfig spiMasterConfig = {
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,                           // SMCLK Clock Source
        48000000,                                                 // SMCLK = DCO = 3MHZ
        48000000,                                                  // SPICLK = 500khz
        EUSCI_B_SPI_MSB_FIRST,                                   // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT, // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,               // High polarity
    };
    // GPIO初始化
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                               GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring SPI in 3wire master mode */
    MAP_SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);
    /* Enable SPI module */
    MAP_SPI_enableModule(EUSCI_B0_BASE);
#else
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, 0xff);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, 0xff);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN4);
#endif
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, 0x0f);
    CS_H;
    BL_L;

    //初始化部分
    RST_H;
    delay_ms(50);
    RST_L;
    delay_ms(50);
    RST_H;
    delay_ms(50);
    CS_L;

    //************* ILI9341初始化 **********//
    writeCmd(0xCF);
    writeData(0x00);
    writeData(0xC1);
    writeData(0X30);
    writeCmd(0xED);
    writeData(0x64);
    writeData(0x03);
    writeData(0X12);
    writeData(0X81);
    writeCmd(0xE8);
    writeData(0x85);
    writeData(0x10);
    writeData(0x7A);
    writeCmd(0xCB);
    writeData(0x39);
    writeData(0x2C);
    writeData(0x00);
    writeData(0x34);
    writeData(0x02);
    writeCmd(0xF7);
    writeData(0x20);
    writeCmd(0xEA);
    writeData(0x00);
    writeData(0x00);
    writeCmd(0xC0);  // Power control
    writeData(0x1B); // VRH[5:0]
    writeCmd(0xC1);  // Power control
    writeData(0x01); // SAP[2:0];BT[3:0]
    writeCmd(0xC5);  // VCM control
    writeData(0x30); // 3F
    writeData(0x30); // 3C
    writeCmd(0xC7);  // VCM control2
    writeData(0XB7);
    writeCmd(0x36); // Memory Access Control
    writeData(0x48);
    writeCmd(0x3A);
    writeData(0x55);
    writeCmd(0xB1);
    writeData(0x00);
    writeData(0x1A);
    writeCmd(0xB6); // Display Function Control
    writeData(0x0A);
    writeData(0xA2);
    writeCmd(0xF2); // 3Gamma Function Disable
    writeData(0x00);
    writeCmd(0x26); // Gamma curve selected
    writeData(0x01);
    writeCmd(0xE0); // Set Gamma
    writeData(0x0F);
    writeData(0x2A);
    writeData(0x28);
    writeData(0x08);
    writeData(0x0E);
    writeData(0x08);
    writeData(0x54);
    writeData(0XA9);
    writeData(0x43);
    writeData(0x0A);
    writeData(0x0F);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeCmd(0XE1); // Set Gamma
    writeData(0x00);
    writeData(0x15);
    writeData(0x17);
    writeData(0x07);
    writeData(0x11);
    writeData(0x06);
    writeData(0x2B);
    writeData(0x56);
    writeData(0x3C);
    writeData(0x05);
    writeData(0x10);
    writeData(0x0F);
    writeData(0x3F);
    writeData(0x3F);
    writeData(0x0F);
    writeCmd(0x2B);
    writeData(0x00);
    writeData(0x00);
    writeData(0x01);
    writeData(0x3f);
    writeCmd(0x2A);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xef);
    writeCmd(0x11); // Exit Sleep
    delay_ms(120);
    writeCmd(0x29); // display on

    writeCmd(0x36);
    writeData((1 << 3) | (0 << 6) | (0 << 7));
}
