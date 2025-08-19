
#include <stdint.h>
#include <stdio.h>

#include "setup/clock.h"
#include "setup/dma.h"
#include "setup/gpio.h"
#include "setup/timer.h"
#include "device_api.h"
#include "setup/stm32l4xx_hal_conf.h"
#include "debug/usart.h"

/**
 * @brief Initialise system clock and required peripherals.
 */
void device_init(void)
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
/**
 * @brief Toggle the onboard status LED.
 */
void device_toggle_led(void)
{
    HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
}

/**
 * @brief Blocking delay.
 * @param time_ms Delay time in milliseconds.
 */
void device_delay_ms(int time_ms)
{
    HAL_Delay(time_ms);
}

//--------------------------------------//
//               ADC APIs               //
//--------------------------------------//
/**
 * @brief Register a callback for ADC conversion complete.
 * 
 * Allows the application to supply a function that will be called
 * when the ADC buffer is ready, keeping drivers and application decoupled.
 *
 * @param callback Function pointer to handle completed ADC data.
 */
void device_register_adc_conv_complete_callback(adc_callback_t callback)
{
    adc_register_callback(callback);
}
/**
 * @brief Start ADC sampling with DMA.
 * @param buffer Pointer to destination buffer.
 * @param buffer_len Number of samples in buffer.
 */
void device_start_adc(uint16_t *buffer, size_t buffer_len)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_TIM_Base_Start(&htim6);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)buffer, buffer_len);
}
/**
 * @brief Stop ADC sampling and DMA.
 */
void device_stop_adc(void)
{
    HAL_TIM_Base_Stop(&htim6);
    HAL_ADC_Stop_DMA(&hadc1);
}
//--------------------------------------//
//              MOTOR PWM               //
//--------------------------------------//
/**
 * @brief Enable motor and set initial speed.
 * @param speed_percent Motor speed as percentage (-100 to 100).
 */
void device_motor_on(int speed_percent)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    device_motor_adjust_speed(speed_percent);
}
/**
 * @brief Disable motor output.
 */

void device_motor_off(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
}

/**
 * @brief Adjust motor speed.
 * @param speed_percent Motor speed as percentage (-100 to 100).
 *        Positive = one direction, negative = reverse.
 *        Values within ±2% are treated as stop.
 */
void device_motor_adjust_speed(int speed_percent)
{
    HAL_PWM_set_pulse_width(speed_percent);
    if (speed_percent > 100) speed_percent = 100;
    if (speed_percent < -100) speed_percent = -100;
    
    if (speed_percent <= 2 && speed_percent >= -2) {
        device_motor_off();
        return;
    }
    
    uint16_t pulse_width_us;
    
    if (speed_percent >= 0) 
        pulse_width_us = 9700 - (speed_percent * 10);
    else 
        pulse_width_us = 9700 + (abs(speed_percent) * 10);
    
}
