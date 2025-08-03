#ifndef AUTOTUNE_H
#define AUTOTUNE_H

#include "arm_math.h"
#include <stdint.h>

#define SIGNAL_BUFF_LEN 1024
#define ADC_BUFF_LEN 2 * SIGNAL_BUFF_LEN

enum
{
    FIRST_HALF,
    SEDOND_HALF
};

extern uint16_t adc_buffer[ADC_BUFF_LEN];
extern float32_t signal_buffer[SIGNAL_BUFF_LEN];

void start_autotune_mode(void);
void autotune_mode(int buffer_section);
void adc_to_guitar_signal(uint16_t *src, float32_t *guitar_signal);

#endif /* AUTOTUNE_H */
