#include "ili9341.h"

namespace LCDScreen
{
   
    Ili9341::Ili9341(SPI_HandleTypeDef *hspi2) : spi(hspi2)
    {

        //usart->PrintBlocking("Initializing screen driver\r\n");

        this->Initialization();
        //usart->PrintBlocking("FiNISHED Initializing screen driver\r\n");
    }

    void Ili9341::SetRotation(ScreenOrientation rot)
    {
        this->SelectCommand(Commands::MemoryAccessControl);
        HAL_Delay(1);

        switch (rot)
        {
        case ScreenOrientation::Vertical:
            this->WriteCommandData(0x40 | 0x08);
            break;
        case ScreenOrientation::Horizontal:
            this->WriteCommandData(0x20 | 0x08);
            break;
        case ScreenOrientation::Vertical2:
            this->WriteCommandData(0x80 | 0x08);
            break;
        case ScreenOrientation::Horizontal2:
            this->WriteCommandData(0x40 | 0x80 | 0x20 | 0x08);
            break;
        default:
            break;
        }
    }

    void Ili9341::FillScreen(uint16_t color)
    {
        this->SetAddress(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        this->DrawColorBurst(color, SCREEN_WIDTH * SCREEN_HEIGHT);
    }

    void Ili9341::SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
    {
        uint8_t buffer[4];
        buffer[0] = x1 >> 8;
        buffer[1] = x1;
        buffer[2] = x2 >> 8;
        buffer[3] = x2;

        this->SelectCommand(Commands::CASET);
        this->WriteCommandData(buffer, sizeof(buffer));

        buffer[0] = y1 >> 8;
        buffer[1] = y1;
        buffer[2] = y2 >> 8;
        buffer[3] = y2;

        this->SelectCommand(Commands::PASET);
        this->WriteCommandData(buffer, sizeof(buffer));
        this->SelectCommand(Commands::MemoryWrite);
    }

    void Ili9341::DrawPixel(uint16_t x, uint16_t y, uint16_t color)
    {
        if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))
            return;

        uint8_t bufferX[4] = {x >> 8, x, (x + 1) >> 8, (x + 1)};
        uint8_t bufferY[4] = {y >> 8, y, (y + 1) >> 8, (y + 1)};
        uint8_t bufferC[2] = {color >> 8, color};

        this->SelectCommand(Commands::CASET);
        this->WriteCommandData(bufferX, sizeof(bufferX));
        this->SelectCommand(Commands::PASET);
        this->WriteCommandData(bufferY, sizeof(bufferY));
        this->SelectCommand(Commands::MemoryWrite);
        this->WriteCommandData(bufferC, sizeof(bufferC));
    }

    void Ili9341::DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
    {
        if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))
            return;

        if ((x + width - 1) >= SCREEN_WIDTH)
        {
            width = SCREEN_WIDTH - x;
        }

        if ((y + height - 1) >= SCREEN_HEIGHT)
        {
            height = SCREEN_HEIGHT - y;
        }

        this->SetAddress(x, y, x + width - 1, y + height - 1);
        this->DrawColorBurst(color, height * width);
    }

    void Ili9341::DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
    {
        if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))
            return;

        if ((x + width - 1) >= SCREEN_WIDTH)
        {
            width = SCREEN_WIDTH - x;
        }
        this->SetAddress(x, y, x + width - 1, y);
        this->DrawColorBurst(color, width);
    }

    void Ili9341::DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color)
    {
        if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))
            return;

        if ((y + height - 1) >= SCREEN_HEIGHT)
        {
            height = SCREEN_HEIGHT - y;
        }
        this->SetAddress(x, y, x, y + height - 1);
        this->DrawColorBurst(color, height);
    }

#pragma region Private Methods
    void Ili9341::Initialization(void)
    {
        // Active LCD.
        this->Enable();
        this->ChipReset();

        this->SelectCommand(Commands::SoftwareReset);

        HAL_Delay(10);

        this->SelectCommand(Commands::PowerControlA);
        this->WriteCommandData(0x39);
        this->WriteCommandData(0x2C);
        this->WriteCommandData(0x00);
        this->WriteCommandData(0x34);
        this->WriteCommandData(0x02);

        this->SelectCommand(Commands::PowerControlB);
        this->WriteCommandData(0x00);
        this->WriteCommandData(0xC1);
        this->WriteCommandData(0x30);

        this->SelectCommand(Commands::DriverTimingControlA);
        this->WriteCommandData(0x85);
        this->WriteCommandData(0x00);
        this->WriteCommandData(0x78);

        this->SelectCommand(Commands::DriverTimingControlC);
        this->WriteCommandData(0x00);
        this->WriteCommandData(0x00);

        this->SelectCommand(Commands::PowerOnSequenceControl);
        this->WriteCommandData(0x64);
        this->WriteCommandData(0x03);
        this->WriteCommandData(0x12);
        this->WriteCommandData(0x81);

        this->SelectCommand(Commands::PumpRatioControl);
        this->WriteCommandData(0x20);

        this->SelectCommand(Commands::PowerControl1);
        this->WriteCommandData(0x23);
        this->SelectCommand(Commands::PowerControl2);
        this->WriteCommandData(0x10);

        this->SelectCommand(Commands::VCOMControl1);
        this->WriteCommandData(0x3E);
        this->WriteCommandData(0x28);
        this->SelectCommand(Commands::VCOMControl2);
        this->WriteCommandData(0x86);

        this->SelectCommand(Commands::MemoryAccessControl);
        this->WriteCommandData(0x48);

        this->SelectCommand(Commands::PixelFormatSet);
        this->WriteCommandData(0x55);

        this->SelectCommand(Commands::FrameControlNormalMode);
        this->WriteCommandData(0x00);
        this->WriteCommandData(0x18);

        this->SelectCommand(Commands::DisplayFunctionControl);
        this->WriteCommandData(0x08);
        this->WriteCommandData(0x82);
        this->WriteCommandData(0x27);

        this->SelectCommand(Commands::Enable3G);
        this->WriteCommandData(0x00);

        this->SelectCommand(Commands::GammaSet);
        this->WriteCommandData(0x01);

        this->SelectCommand(Commands::PositiveGammaCorrection);
        this->WriteCommandData(0x0F);
        this->WriteCommandData(0x31);
        this->WriteCommandData(0x2B);
        this->WriteCommandData(0x0C);
        this->WriteCommandData(0x0E);
        this->WriteCommandData(0x08);
        this->WriteCommandData(0x4E);
        this->WriteCommandData(0xF1);
        this->WriteCommandData(0x37);
        this->WriteCommandData(0x07);
        this->WriteCommandData(0x10);
        this->WriteCommandData(0x03);
        this->WriteCommandData(0x0E);
        this->WriteCommandData(0x09);
        this->WriteCommandData(0x00);

        this->SelectCommand(Commands::NegativeGammaCorrection);
        this->WriteCommandData(0x00);
        this->WriteCommandData(0x0E);
        this->WriteCommandData(0x14);
        this->WriteCommandData(0x03);
        this->WriteCommandData(0x11);
        this->WriteCommandData(0x07);
        this->WriteCommandData(0x31);
        this->WriteCommandData(0xC1);
        this->WriteCommandData(0x48);
        this->WriteCommandData(0x08);
        this->WriteCommandData(0x0F);
        this->WriteCommandData(0x0C);
        this->WriteCommandData(0x31);
        this->WriteCommandData(0x36);
        this->WriteCommandData(0x0F);

        this->SelectCommand(Commands::SleepOut);
        HAL_Delay(100);
        this->SelectCommand(Commands::DisplayON);
    }

    void Ili9341::SelectCommand(Commands command)
    {
        uint8_t cmd = (uint8_t)command;
        this->DCX(GPIO_PinState::GPIO_PIN_RESET);
        this->CS(GPIO_PinState::GPIO_PIN_RESET);
        this->SPI_Tx(&cmd, 1);
    }

    void Ili9341::WriteCommandData(uint8_t data)
    {
        this->DCX(GPIO_PinState::GPIO_PIN_SET);
        this->CS(GPIO_PinState::GPIO_PIN_RESET);
        this->SPI_Tx(&data, 1);
    }

    void Ili9341::WriteCommandData(uint8_t *buffer, uint16_t length)
    {
        this->DCX(GPIO_PinState::GPIO_PIN_SET);
        this->CS(GPIO_PinState::GPIO_PIN_RESET);
        this->SPI_Tx(buffer, length);
    }

    void Ili9341::CS(GPIO_PinState pinState)
    {
        HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, pinState);
    }

    void Ili9341::DCX(GPIO_PinState pinState)
    {
        HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX_PIN, pinState);
    }

    void Ili9341::ResetPin(GPIO_PinState pinState)
    {
        HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, pinState);
    }

    /**
     * Reset LCD, is Active LOW
     */
    void Ili9341::ChipReset()
    {
        this->ResetPin(GPIO_PinState::GPIO_PIN_RESET);
        HAL_Delay(10);
        this->CS(GPIO_PinState::GPIO_PIN_RESET);
        HAL_Delay(10);
        this->ResetPin(GPIO_PinState::GPIO_PIN_SET);
        this->CS(GPIO_PinState::GPIO_PIN_SET);
    }

    void Ili9341::Enable()
    {
        HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
    }

    void Ili9341::HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
    {
        /* Deselect when Tx Complete */
        if (hspi == this->spi)
        {
            HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
        }
    }

    void Ili9341::SPI_Tx(uint8_t data)
    {
        while (!__HAL_SPI_GET_FLAG(this->spi, SPI_FLAG_TXE))
            ;
        HAL_SPI_Transmit_DMA(this->spi, &data, 1);
    }

    void Ili9341::SPI_Tx(uint8_t *data, uint16_t len)
    {
        while (!__HAL_SPI_GET_FLAG(this->spi, SPI_FLAG_TXE))
            ;
        HAL_SPI_Transmit_DMA(this->spi, data, len);
    }

    void Ili9341::DrawColorBurst(uint16_t color, uint32_t size)
    {
        uint32_t BufferSize = 0;

        if ((size * 2) < BURST_MAX_SIZE)
        {
            BufferSize = size;
        }
        else
        {
            BufferSize = BURST_MAX_SIZE;
        }

        this->DCX(GPIO_PinState::GPIO_PIN_SET);
        this->CS(GPIO_PinState::GPIO_PIN_RESET);

        uint8_t chifted = color >> 8;
        uint8_t BurstBuffer[BufferSize];

        for (uint32_t j = 0; j < BufferSize; j += 2)
        {
            BurstBuffer[j] = chifted;
            BurstBuffer[j + 1] = color;
        }

        uint32_t SendingSize = size * 2;
        uint32_t SendingInBlock = SendingSize / BufferSize;
        uint32_t RemainderFromBlock = SendingSize % BufferSize;

        if (SendingInBlock != 0)
        {
            for (uint32_t j = 0; j < (SendingInBlock); j++)
            {
                HAL_SPI_Transmit(this->spi, BurstBuffer, BufferSize, 10);
            }
        }

        HAL_SPI_Transmit(this->spi, BurstBuffer, RemainderFromBlock, 10);
        this->CS(GPIO_PinState::GPIO_PIN_SET);
    }

#pragma endregion

    Ili9341::~Ili9341()
    {
        // BSP_SPI2_DeInit();
    }
}
