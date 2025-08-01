

#include "system/clock_config.h"
#include "system/peripherals.h"
#include "board_interface.h"
#include "hal/stm32l4xx_hal_conf.h"
#include "debug/usart.h"
#include <stdint.h>
#include <stdio.h>
#include "stm32l4xx_hal.h"


uint16_t adc_buffer[ADC_BUF_LEN];

void init_board(void)
{
    // Initialises
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM6_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    // Start up ADC and its trigger TIM6
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, ADC_BUF_LEN);
    HAL_TIM_Base_Start(&htim6);
}

void toggle_and_wait_led(int wait_time_ms)
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
    HAL_Delay(wait_time_ms);
}

void board_copy_adc_samples(float *dest, uint16_t len)
{
    if (len > ADC_BUF_LEN)
        len = ADC_BUF_LEN;

    for (uint16_t i = 0; i < len; i++)
    {
        dest[i] = (float)adc_buffer[i] + 0.1f;  // Add 0.1 to see decimal
    }
}
