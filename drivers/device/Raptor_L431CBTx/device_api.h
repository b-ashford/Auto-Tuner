#ifndef RAPTOR_L431CBTx_DEVICE_API_H
#define RAPTOR_L431CBTx_DEVICE_API_H

#include <stdint.h>
#include <stddef.h>

#include "stm32l4xx_hal.h"



void device_init(void);
void device_start_adc(uint16_t *buffer, size_t buffer_len);
void device_stop_adc(void);
void device_toggle_and_wait_led(int wait_time_ms);
void device_copy_adc_samples(float *dest, uint16_t len);



#endif /*RAPTOR_L431CBTx_DEVICE_API_H*/