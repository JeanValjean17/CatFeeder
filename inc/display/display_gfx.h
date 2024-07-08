
#ifndef DISPLAY_GFX_H
#define DISPLAY_GFX_H

#include "stm32l0xx_hal.h"
#include "ili9341.h"

namespace LCDScreen
{
    typedef enum
    {
        BLACK = 0x0000,
        NAVY = 0x000F,
        DARKGREEN = 0x03E0,
        DARKCYAN = 0x03EF,
        MAROON = 0x7800,
        PURPLE = 0x780F,
        OLIVE = 0x7BE0,
        LIGHTGREY = 0xC618,
        DARKGREY = 0x7BEF,
        BLUE = 0x001F,
        GREEN = 0x07E0,
        CYAN = 0x07FF,
        RED = 0xF800,
        MAGENTA = 0xF81F,
        YELLOW = 0xFFE0,
        WHITE = 0xFFFF,
        ORANGE = 0xFD20,
        GREENYELLOW = 0xAFE5,
        PINK = 0xF81F
    } Color;

    class DisplayGFX
    {

    public:
        DisplayGFX(Ili9341 *displayDriver);
        void RenderLoop();
        ~DisplayGFX();

    private:
        Ili9341 *driver;
        ScreenOrientation orientation;
        void SetScreenOrientation(ScreenOrientation orientation);
        void DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
        void DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
        void DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
        void DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
        void DrawChar(int16_t ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
        void DrawText(const char *str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
        void DrawImage(const uint8_t *image, uint8_t orientation);
    };
}
#endif /*DISPLAY_GFX_H*/
