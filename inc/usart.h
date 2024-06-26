#ifndef __USART_H
#define __USART_H

#include "stm32l0xx_hal.h"
#include <stdio.h>

#define DEBUG_BUFFER_SIZE 1024

namespace Drivers
{

    class Usart
    {
    public:
        Usart(UART_HandleTypeDef *huart);
        ~Usart();
        void PrintBlocking(const char *characterBufferToSend, ...);

    private:
        UART_HandleTypeDef *UsartRef;     
        void VPrint(const char * string, __va_list args);   
        void InitGpioUartPort(void);
    };
}

#endif /*__USART_H*/
