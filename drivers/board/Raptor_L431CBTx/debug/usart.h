#ifndef __USART_H__
#define __USART_H__

#include "stm32l4xx_hal.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

void MX_USART1_UART_Init(void);
void uart_dma_test_float_sine_wave(void);
HAL_StatusTypeDef uart_dma_float_buffer(float *buff, int len);
HAL_StatusTypeDef uart_dma_print_string(const char *str);

#endif /* __USART_H__ */
