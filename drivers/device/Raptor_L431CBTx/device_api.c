
#include <stdint.h>
#include <stdio.h>

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


//--------------------------------------//
//              UTIL APIs               //
//--------------------------------------//
void device_toggle_led(void)
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
}

void device_delay_ms(int time_ms)
{
    HAL_Delay(time_ms);
}



//--------------------------------------//
//               ADC APIs               //
//--------------------------------------//
void device_start_adc(uint16_t *buffer, size_t buffer_len)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)buffer, buffer_len);
}

void device_stop_adc(void)
{
    HAL_TIM_Base_Stop(&htim6);
    HAL_ADC_Stop_DMA(&hadc1);
}
/* Pass a function from the main application which can process full adc buffer.
  Keeps main application and device drivers seperated */
void device_register_adc_conv_complete_callback(adc_callback_t callback)
{
    adc_register_callback(callback);
}