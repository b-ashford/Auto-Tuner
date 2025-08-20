
#ifndef RAPTOR_L431CBTx_TIMER_H
#define RAPTOR_L431CBTx_TIMER_H


#include "stm32l4xx_hal.h"

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim1;

void MX_TIM6_Init(void);
void MX_TIM1_Init(void);
void HAL_PWM_set_pulse_width(int pulse_width_us); 

#endif /*RAPTOR_L431CBTx_TIMER_H*/