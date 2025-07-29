

#include "system/clock_config.h"
#include "system/peripherals.h"
#include "board_interface.h"
#include "hal/stm32l4xx_hal_conf.h"
#include "debug/usart.h"
#include <stdint.h>
#include <stdio.h>


uint16_t adc_buffer[ADC_BUF_LEN];

void init_board(void)
{
    HAL_Init();
    SystemClock_Config();  
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);  
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, ADC_BUF_LEN); 
}

void toggle_and_wait_led(int wait_time_ms)
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
    HAL_Delay(wait_time_ms);
}


void board_copy_adc_samples(float *dest, uint16_t len)
{
    if (len > ADC_BUF_LEN) len = ADC_BUF_LEN; 

    for (uint16_t i = 0; i < len; i++) {
        dest[i] = (float)adc_buffer[i];
    }
}

void board_send_floats_uart(const float *data, size_t len)
{
    // Convert to a temporary string buffer (CSV style)
    static char tx_buf[256]; // send in small chunks to avoid blocking too long
    int n;

    for (size_t i = 0; i < len; i++)
    {
        n = snprintf(tx_buf, sizeof(tx_buf), "%.3f\n", data[i]);
        HAL_UART_Transmit(&huart1, (uint8_t *)tx_buf, n, HAL_MAX_DELAY);
    }
}