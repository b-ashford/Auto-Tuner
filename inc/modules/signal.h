#ifndef SIGNAL_H
#define SIGNAL_H

#include "arm_math.h"
#include <stdint.h>

#define BLOCK_SIZE   1024
#define DC_BIAS      2280
#define THRESHOLD    600
#define TABLE_SIZE   5    // sliding window

void signal_init(void);
void adc_to_signal(uint16_t *src, float32_t *signal);
void get_frequency(float32_t *signal, float32_t target_freq, float32_t *out_freq);
float32_t get_error_in_cents(float32_t curr_freq, float32_t target_freq);
int init_table(int pitch_up_lim, int pitch_low_lim, float32_t target_freq);

#endif // SIGNAL_H