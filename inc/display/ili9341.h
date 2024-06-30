#ifndef __ILI9341_H
#define __ILI9341_H

#include "stdint.h"
#include "stm32l0xx_hal.h"
#include "usart.h"

namespace LCDScreen
{

#define LCD_CS_PORT GPIOC
#define LCD_CS_PIN GPIO_PIN_7
#define LCD_DCX_PORT GPIOB
#define LCD_DCX_PIN GPIO_PIN_9
#define LCD_RST_PORT GPIOB
#define LCD_RST_PIN GPIO_PIN_8

    const uint16_t BURST_MAX_SIZE = 500;
    const uint16_t SCREEN_HEIGHT = 240;
    const uint16_t SCREEN_WIDTH = 320;

#pragma region Registers Definitions
    typedef enum
    {
        NOP = 0x00,
        SoftwareReset = 0x01,
        ReadDisplayID = 0x04,
        ReadDisplayStatus = 0x09,
        ReadPowerMode = 0x0A,
        ReadMADCTL = 0x0B,
        ReadPixelFormat = 0x0C,
        ReadImageFormat = 0x0D,
        ReadSignalMode = 0x0E,
        ReadSelfDiagnostic = 0x0F,
        EnterSleepMode = 0x10,
        SleepOut = 0x11,
        PartialModeON = 0x12,
        NormalDisplayModeON = 0x13,
        DisplayInversionOFF = 0x20,
        DisplayInversionON = 0x21,
        GammaSet = 0x26,
        DisplayOFF = 0x28,
        DisplayON = 0x29,
        CASET = 0x2A, // Column Address set
        PASET = 0x2B, // Page Address Set
        MemoryWrite = 0x2C,
        ColorSet = 0x2D,
        MemoryRead = 0x2E,
        PartialArea = 0x30,
        VerticalScrollDef = 0x33,
        TearingEffectLineOFF = 0x34,
        TearingEffectLineON = 0x35,
        MemoryAccessControl = 0x36,
        VerticalScrollStartAddr = 0x37,
        IdleModeOFF = 0x38,
        IdleModeON = 0x39,
        PixelFormatSet = 0x3A,
        WriteMemoryContinue = 0x3C,
        ReadMemoryContinue = 0x3E,
        SetTearScanline = 0x44,
        GetScanline = 0x45,
        WriteDisplayBrightness = 0x51,
        ReadDisplayBrightness = 0x52,
        WriteCTRLDisplay = 0x53,
        ReadCTRLDisplay = 0x54,
        WriteCABC = 0x55, // Write Content Adaptive Brightness Control
        ReadCABC = 0x56,  // Read Content Adaptive Brightness Control
        WriteMinCABC = 0x5E,
        ReadMinCABC = 0x5F,
        ReadID1 = 0xDA,
        ReadID2 = 0xDB,
        ReadID3 = 0xDC,
        RGBSignalControl = 0xB0,
        FrameControlNormalMode = 0xB1,
        FrameControlIdleMode = 0xB2,
        FrameControlPartialMode = 0xB3,
        DisplayInversionControl = 0xB4,
        BlankingPorchControl = 0xB5,
        DisplayFunctionControl = 0xB6,
        EntryModeSet = 0xB7,
        BacklightControl1 = 0xB8,
        BacklightControl2 = 0xB9,
        BacklightControl3 = 0xBA,
        BacklightControl4 = 0xBB,
        BacklightControl5 = 0xBC,
        BacklightControl6 = 0xBD,
        BacklightControl7 = 0xBE,
        BacklightControl8 = 0xBF,
        PowerControl1 = 0xC0,
        PowerControl2 = 0xC1,
        VCOMControl1 = 0xC5,
        VCOMControl2 = 0xC7,
        NVMemoryWrite = 0xD0,
        NVMemmoryProtectionKey = 0xD1,
        NVMemmoryStatusRead = 0xD2,
        ReadID4 = 0xD3,
        PositiveGammaCorrection = 0xE0,
        NegativeGammaCorrection = 0xE1,
        DigitalGammaControl1 = 0xE2,
        DigitalGammaControl2 = 0xE3,
        InterfaceControl = 0xF6,
        PowerControlA = 0xCB,
        PowerControlB = 0xCF,
        DriverTimingControlA = 0xE8,
        DriverTimingControlB = 0xE9,
        DriverTimingControlC = 0xEA,
        PowerOnSequenceControl = 0xED,
        Enable3G = 0xF2,
        PumpRatioControl = 0xF7
    } Commands;

    typedef enum
    {
        Vertical,
        Horizontal,
        Vertical2,
        Horizontal2
    } ScreenOrientation;

#pragma endregion

    class Ili9341
    {
    public:
        Ili9341(Drivers::Usart *usart, SPI_HandleTypeDef *hspi2);
        ~Ili9341();
        void SetRotation(ScreenOrientation rot);
        void SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
        void DrawPixel(uint16_t x, uint16_t y, uint16_t color);
        void DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
        void DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color);
        void DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color);
        void WriteCommandData(uint8_t *buffer, uint16_t length);
        void DrawColorBurst(uint16_t color, uint32_t size);
        void FillScreen(uint16_t color);

    private:
        Drivers::Usart *debug;
        SPI_HandleTypeDef *spi;
        void Initialization(void);
        void Enable(void);
        void SelectCommand(Commands command);
        void WriteCommandData(uint8_t data);
        void CS(GPIO_PinState);
        void DCX(GPIO_PinState);
        void ResetPin(GPIO_PinState pinState);
        void ChipReset(void);
        void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
        void SPI_Tx(uint8_t data);
        void SPI_Tx(uint8_t *data, uint16_t len);
    };
}

#endif /* __ILI9341_H */
