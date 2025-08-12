#ifndef AUTOTUNE_H
#define AUTOTUNE_H

#include "arm_math.h"
#include <stdint.h>

#define SIGNAL_BUFF_LEN 1024
#define ADC_BUFF_LEN 2 * SIGNAL_BUFF_LEN
#define ADC_SAMPLE_RATE 8000
#define LAG_STOP_SEARCH 150
#define CLARITY_RATIO 0.9f
#define PEAK_THRESHOLD 0.5f
enum
{
    FIRST_HALF,
    SEDOND_HALF
};

extern uint16_t adc_buffer[ADC_BUFF_LEN];
extern float32_t signal_buffer_f32[SIGNAL_BUFF_LEN];

void start_autotune_mode(void);
void autotune_mode(int buffer_section);


#endif /* AUTOTUNE_H */
