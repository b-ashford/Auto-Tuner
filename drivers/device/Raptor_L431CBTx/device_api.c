
#include <stdint.h>
#include <stdio.h>


#include "setup/adc.h"
#include "setup/clock.h"
#include "setup/dma.h"
#include "setup/gpio.h"
#include "setup/timer.h"
#include "device_api.h"
#include "setup/stm32l4xx_hal_conf.h"
#include "debug/usart.h"




void device_init(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM6_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
}

void device_start_adc(uint16_t *buffer, size_t buffer_len)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)buffer, buffer_len);
    HAL_TIM_Base_Start(&htim6);
}

void device_stop_adc(void)
{
   HAL_TIM_Base_Stop(&htim6);   
   HAL_ADC_Stop_DMA(&hadc1);        
}

void device_toggle_and_wait_led(int wait_time_ms)
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
    HAL_Delay(wait_time_ms);
}

