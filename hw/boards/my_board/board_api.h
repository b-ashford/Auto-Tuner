#ifndef MY_BOARD_API_H
#define MY_BOARD_API_H

#include <stddef.h> 
#include <stdint.h>

//--------------------------------------------------//
//                  BOARD INIT                      //
//--------------------------------------------------//

/**
 * @brief Initialize board peripherals (GPIO, ADC, timers, etc.)
 */
void board_init(void);


//--------------------------------------------------//
//                  UTIL APIs                       //
//--------------------------------------------------//

/**
 * @brief Toggle the on-board status LED.
 */
void board_toggle_led(void);

/**
 * @brief Blocking delay in milliseconds.
 *
 * @param time_ms Duration to delay (ms).
 */
void board_delay_ms(int time_ms);


//--------------------------------------------------//
//                  ADC APIs                        //
//--------------------------------------------------//

/**
 * @typedef adc_callback_t
 * @brief Callback function type for ADC conversion complete events.
 *
 * The driver will invoke this function once an ADC conversion has finished.
 *
 * @param value  Latest ADC sample value (integer).
 */
typedef void (*adc_callback_t)(int);

/**
 * @brief Register a callback for ADC conversion complete events.
 *
 * @param callback Function to call when ADC completes.
 */
void board_register_adc_conv_complete_callback(adc_callback_t callback);

/**
 * @brief Start ADC conversions into a buffer.
 *
 * @param buffer     Pointer to destination buffer.
 * @param buffer_len Number of samples to acquire.
 */
void board_start_adc(uint16_t *buffer, size_t buffer_len);

/**
 * @brief Stop ADC conversions.
 */
void board_stop_adc(void);


//--------------------------------------------------//
//                  MOTOR PWM                       //
//--------------------------------------------------//

/**
 * @brief Adjust motor speed in percentage of full scale.
 *
 * @param speed_percent Range: -100% to +100%.
 */
void board_motor_adjust_speed(int speed_percent);

/**
 * @brief Enable/start motor output.
 */
void board_motor_on(void);

/**
 * @brief Disable/stop motor output.
 */
void board_motor_off(void);


#endif // MY_BOARD_API_H