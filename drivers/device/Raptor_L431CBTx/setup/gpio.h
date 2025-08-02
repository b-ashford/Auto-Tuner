
#ifndef RAPTOR_L431CBTx_GPIO_H
#define RAPTOR_L431CBTx_GPIO_H


#include "stm32l4xx_hal.h"

#define BLUE_LED_Pin GPIO_PIN_15
#define BLUE_LED_GPIO_Port GPIOA

void MX_GPIO_Init(void);

#endif /*RAPTOR_L431CBTx_GPIO_H*/