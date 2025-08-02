
#ifndef RAPTOR_L431CBTx_ADC_H
#define RAPTOR_L431CBTx_ADC_H


#include "stm32l4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

typedef void (*adc_callback_t)(void);

void adc_register_callback(adc_callback_t callback);
void adc_start_with_buffer(uint16_t *buffer, size_t buffer_len);


void MX_ADC1_Init(void);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1);




#endif /*RAPTOR_L431CBTx_ADC_H*/