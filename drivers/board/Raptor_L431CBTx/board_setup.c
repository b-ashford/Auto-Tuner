

#include "system/clock_config.h"
#include "system/peripherals.h"
#include "board_setup.h"
#include "hal/stm32l4xx_hal_conf.h"
#include "debug/usart.h"

void init_board(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
}

void toggle_and_wait_led(int wait_time_ms)
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
    HAL_Delay(wait_time_ms);
}