
#ifndef RAPTOR_L431CBTx_ADC_H
#define RAPTOR_L431CBTx_ADC_H


#include "stm32l4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

void MX_ADC1_Init(void);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1);

#endif /*RAPTOR_L431CBTx_ADC_H*/