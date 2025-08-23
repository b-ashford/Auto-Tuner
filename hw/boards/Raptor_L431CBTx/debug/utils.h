#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include "arm_math.h"
#include "usart.h"

#define ALIGN4 __attribute__((aligned(4))) 
#define SIGNAL_BUFF_LEN 1024


HAL_StatusTypeDef debug_uart_dma_print_string(const char *str);
HAL_StatusTypeDef debug_uart_dma_uint16_buffer(uint16_t *buff, int len, uint32_t wait_ms);
HAL_StatusTypeDef debug_uart_dma_float_buffer(float *buff, int len, uint32_t wait_ms);

void debug_dwt_enable(void);
uint32_t debug_start_cycle_count(void);
float debug_end_cycle_us(uint32_t start_cycles, float clock_mhz);

void init_debug(void);
void SWO_Init(void);
void ITM_Init(void);


#endif /* DEBUG_UTILS_H */