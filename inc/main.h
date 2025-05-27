#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);



#define Button2_Pin GPIO_PIN_6
#define Button2_GPIO_Port GPIOA
#define Button3_Pin GPIO_PIN_7
#define Button3_GPIO_Port GPIOA
#define Switch_High_Pin GPIO_PIN_15
#define Switch_High_GPIO_Port GPIOB
#define Switch_Int_Pin GPIO_PIN_8
#define Switch_Int_GPIO_Port GPIOA
#define Switch_Int_EXTI_IRQn EXTI9_5_IRQn
#define LED_BLUE_Pin GPIO_PIN_15
#define LED_BLUE_GPIO_Port GPIOA
#define Button1_Pin GPIO_PIN_8
#define Button1_GPIO_Port GPIOB
#define Button2_Vcc_Pin GPIO_PIN_9
#define Button2_Vcc_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
