#ifndef RAPTOR_L431CBTx_BOARD_API_H
#define RAPTOR_L431CBTx_BOARD_API_H

#include <stdint.h>
#include <stddef.h>
#include "board_api.h"
#include "stm32l4xx_hal.h"
#include "setup/adc.h"

#define ALIGN4 __attribute__((aligned(4))) // DMA must be aligned

/// @brief Initialise peripherals
/// @param
void board_init(void);

//--------------------------------------//
//              UTIL APIs               //
//--------------------------------------//

/// @brief Toggle LED on device.
/// @param
void board_toggle_led(void);

/// @brief Add a blocking delay
/// @param time_ms
void board_delay_ms(int time_ms);

//--------------------------------------//
//               ADC APIs               //
//--------------------------------------//

/// @brief Registor the function for the ADC to callback
/// @param callback
/// @note The ADC driver notifies the app via interrupts.
///       By registering a callback here, the driver knows which
///       application function to call when a conversion completes.
///       This keeps driver logic and application logic separate.
void board_register_adc_conv_complete_callback(adc_callback_t callback);

/// @brief Begin ADC conversions
/// @param buffer
/// @param buffer_len
void board_start_adc(uint16_t *buffer, size_t buffer_len);

/// @brief Stop ADC conversions
/// @param
void board_stop_adc(void);

//--------------------------------------//
//              MOTOR PWM               //
//--------------------------------------//

/// @brief Adjust motor speed and direction
/// @param speed_percent Clamps values to the range [-100, 100]%
void board_motor_adjust_speed(int speed_percent);

/// @brief Start PWM output
/// @param
void board_motor_on(void);

/// @brief Stops PWM output
/// @param
void board_motor_off(void);

#endif // RAPTOR_L431CBTx_BOARD_API_H