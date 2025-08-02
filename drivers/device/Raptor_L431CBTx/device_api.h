#ifndef RAPTOR_L431CBTx_DEVICE_API_H
#define RAPTOR_L431CBTx_DEVICE_API_H

#include <stdint.h>
#include <stddef.h>
#include "device_api.h"
#include "stm32l4xx_hal.h"
#include "setup/adc.h"

void device_init(void);

//--------------------------------------//
//              UTIL APIs               //
//--------------------------------------//
void device_toggle_led(void);
void device_delay_ms(int time_ms);

//--------------------------------------//
//               ADC APIs               //
//--------------------------------------//
void device_register_adc_conv_complete_callback(adc_callback_t callback);
void device_start_adc(uint16_t *buffer, size_t buffer_len);
void device_stop_adc(void);

#endif /*RAPTOR_L431CBTx_DEVICE_API_H*/