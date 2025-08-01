#ifndef RAPTOR_L431CBTx_BOARD_INTERFACE_H
#define RAPTOR_L431CBTx_BOARD_INTERFACE_H

#include <stdint.h>
#include <stddef.h>
#include "stm32l4xx_hal.h"

#define ADC_BUF_LEN 1024

extern uint16_t adc_buffer[ADC_BUF_LEN];

void init_board(void);
void toggle_and_wait_led(int wait_time_ms);
void board_copy_adc_samples(float *dest, uint16_t len);
void MX_TIM6_Init(void);


#endif /*RAPTOR_L431CBTx_BOARD_INTERFACE_H*/