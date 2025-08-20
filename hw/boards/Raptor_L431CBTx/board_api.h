#ifndef RAPTOR_L431CBTx_BOARD_API_H
#define RAPTOR_L431CBTx_BOARD_API_H

#include <stdint.h>
#include <stddef.h>
#include "board_api.h"
#include "stm32l4xx_hal.h"
#include "setup/adc.h"

#define ALIGN4 __attribute__((aligned(4))) // DMA must be aligned

void board_init(void);

//--------------------------------------//
//              UTIL APIs               //
//--------------------------------------//
void board_toggle_led(void);
void board_delay_ms(int time_ms);

//--------------------------------------//
//               ADC APIs               //
//--------------------------------------//
void board_register_adc_conv_complete_callback(adc_callback_t callback);
void board_start_adc(uint16_t *buffer, size_t buffer_len);
void board_stop_adc(void);

//--------------------------------------//
//              MOTOR PWM               //
//--------------------------------------//
// [-100%, 100%]
void board_motor_adjust_speed(int speed_percent);
void board_motor_on(int speed_percent);
void board_motor_off(void);
#endif /*RAPTOR_L431CBTx_BOARD_API_H*/