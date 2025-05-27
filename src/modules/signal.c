#include "modules/signal.h"
#include "modules/mpm.h"
#include <math.h>
#include <stdlib.h>

// External references from main.c
extern volatile int callback_state;
extern float32_t guitar_signal[BLOCK_SIZE];
extern uint16_t adc_buff[2 * BLOCK_SIZE];

// IIR filter instance and state
static arm_biquad_casd_df1_inst_f32 iir_inst;
static float iir_state[4];
static const float iir_taps[5] = {
    0.997987115675119f, -1.995974231350238f, 0.997987115675119f,
    1.995970179642828f, -0.995978283057647f
};

// Sliding window buffer
static float32_t pitch_table[TABLE_SIZE];
static int table_index = 0;

void signal_init(void) {
    arm_biquad_cascade_df1_init_f32(&iir_inst, 1, (float32_t *)iir_taps, iir_state);
}

void adc_to_signal(uint16_t *src, float32_t *signal) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        signal[i] = (float32_t)src[i] - DC_BIAS;
    }
    arm_biquad_cascade_df1_f32(&iir_inst, signal, signal, BLOCK_SIZE);
}

void get_frequency(float32_t *signal, float32_t target_freq, float32_t *out_freq) {
    if (callback_state) {
        float32_t curr_freq = 0;
        mpm_mcleod_pitch_method_f32(signal, &curr_freq);
        callback_state = 0;
        *out_freq = ceilf(curr_freq * 4.0f) / 4.0f;
    }
}

float32_t get_error_in_cents(float32_t curr_freq, float32_t target_freq) {
    return roundf(1200.0f * log2f(curr_freq / target_freq));
}

static void iterate_buffer(void) {
    table_index = (table_index + 1) % TABLE_SIZE;
}

static float get_min_in_buffer(void) {
    float32_t min = pitch_table[0];
    for (int i = 1; i < TABLE_SIZE; i++) {
        if (pitch_table[i] < min) min = pitch_table[i];
    }
    return min;
}

int init_table(int pitch_up_lim, int pitch_low_lim, float32_t target_freq) {
    // fill buffer until last slot out-of-bounds
    if (pitch_table[(table_index + TABLE_SIZE - 1) % TABLE_SIZE] < pitch_low_lim ||
        pitch_table[(table_index + TABLE_SIZE - 1) % TABLE_SIZE] > pitch_up_lim) {
        float32_t freq = 0;
        get_frequency(guitar_signal, target_freq, &freq);
        if (freq > pitch_low_lim && freq < pitch_up_lim) {
            pitch_table[table_index] = freq;
            iterate_buffer();
            return 1;
        }
    }
    return 0;
}
