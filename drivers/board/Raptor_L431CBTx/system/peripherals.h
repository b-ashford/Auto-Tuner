#ifndef __RAPTOR_L431CBTx_PERIPHERALS_H
#define __RAPTOR_L431CBTx_PERIPHERALS_H

#include "stm32l4xx_hal.h"

#define BLUE_LED_Pin GPIO_PIN_15
#define BLUE_LED_GPIO_Port GPIOA

//--------------------------------------//
//                 GPIO                 //
//--------------------------------------//
void MX_GPIO_Init(void);

//--------------------------------------//
//                 DMA                  //
//--------------------------------------//
void MX_DMA_Init(void);

//--------------------------------------//
//                 ADC                  //
//--------------------------------------//
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle);
void MX_ADC1_Init(void);

#endif /*__RAPTOR_L431CBTx_PERIPHERALS_H*/