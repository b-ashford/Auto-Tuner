
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "setup/clock.h"
#include "setup/dma.h"
#include "setup/gpio.h"
#include "setup/timer.h"
#include "board_api.h"
#include "setup/stm32l4xx_hal_conf.h"
#include "debug/usart.h"

void board_init(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM6_Init();
    MX_TIM1_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
}

//--------------------------------------//
//              UTIL APIs               //
//--------------------------------------//
void board_toggle_led(void)
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
}

void board_delay_ms(int time_ms)
{
    HAL_Delay(time_ms);
}

//--------------------------------------//
//               ADC APIs               //
//--------------------------------------//
/* Pass a function from the main application which can process full adc buffer.
  Keeps main application and device drivers seperated */
void board_register_adc_conv_complete_callback(adc_callback_t callback)
{
    adc_register_callback(callback);
}

void board_start_adc(uint16_t *buffer, size_t buffer_len)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)buffer, buffer_len);
}

void board_stop_adc(void)
{
    HAL_TIM_Base_Stop(&htim6);
    HAL_ADC_Stop_DMA(&hadc1);
}
//--------------------------------------//
//              MOTOR PWM               //
//--------------------------------------//
void board_motor_on()
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    board_motor_adjust_speed(0);
}
void board_motor_off(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
}

void board_motor_adjust_speed(int speed_percent)
{
    if (speed_percent > 100)
        speed_percent = 100;
    else if (speed_percent < -100)
        speed_percent = -100;

    uint16_t pulse_width_us = 0;
    if (speed_percent >= -2 && speed_percent <= 2)
        pulse_width_us = 9700;

    else if (speed_percent > 0)
    {
        const float x0 = 110.0f;
        const float alpha = 8961.319958882617f;
        const float beta = -147.08687298804568f;

        float x = (float)speed_percent;
        float y = alpha - beta * logf(x0 - x);

        if (y < 0.0f)
            y = 0.0f;
        if (y > 65535.0f)
            y = 65535.0f;
        pulse_width_us = (uint16_t)(y + 0.5f);
    }
    else
    {
        const float x0n = -110.0f;
        const float alphan = 10627.9f;
        const float betan = 185.8f;

        float x = (float)speed_percent;
        float y = alphan - betan * logf(x - x0n);

        if (y < 0.0f)
            y = 0.0f;
        if (y > 65535.0f)
            y = 65535.0f;
        pulse_width_us = (uint16_t)(y + 0.5f);
    }

    HAL_PWM_set_pulse_width(pulse_width_us);
}