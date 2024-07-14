#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l0xx_hal.h"

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_7
#define LCD_CS_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define MEM_CS_Pin GPIO_PIN_6
#define MEM_CS_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_8
#define LCD_RESET_GPIO_Port GPIOB
#define LCD_DCX_Pin GPIO_PIN_9
#define LCD_DCX_GPIO_Port GPIOB
#define Screen_Key_UP_Pin GPIO_PIN_10
#define Screen_Key_UP_GPIO_Port GPIOC
#define Screen_Key_CENTER_Pin GPIO_PIN_11
#define Screen_Key_CENTER_GPIO_Port GPIOC
#define Screen_Key_RIGHT_Pin GPIO_PIN_12
#define Screen_Key_RIGHT_GPIO_Port GPIOC
#define Screen_Key_LEFT_Pin GPIO_PIN_2
#define Screen_Key_LEFT_GPIO_Port GPIOD
#define Screen_Key_DOWN_Pin GPIO_PIN_13
#define Screen_Key_DOWN_GPIO_Port GPIOC

void SystemClock_Config(void);
void Error_Handler(void);
void MX_GPIO_Init(void);
void MX_SPI2_Init(void);
void MX_DMA_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
