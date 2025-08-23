
#include "autotune/dsp.h"
#include "autotune/params.h"
#include "autotune/mpm.h"
#include "autotune/music.h"
#include "debug/utils.h"
#include <float.h>

float target = 0.0f;
float detected = 0.0f;

FrameEnergyHistory feh = {
    .len = FEH_LEN,
    .idx = 0,
    .gate_open = false,
    .k_on = DSP_KON,
    .k_off = DSP_KOFF};

static inline float get_percentile(float32_t *values, int len, float percentile);
static inline float get_min(float32_t *values, int len);

// Init with max since we take the min of the history
void dsp_init(void)
{
    for (int i = 0; i < feh.len; i++)
        feh.history[i] = FLT_MAX;
}

inline bool dsp_should_process(float32_t *signal, int len)
{

    float32_t energy = 0.0f;
    arm_power_f32(signal, len, &energy);

    float32_t floor = get_percentile(feh.history, feh.len, 20);
    
    if (floor < 1e-9f)
        floor = 1e-9f;

    if (energy > feh.k_on * floor)
        feh.gate_open = true;
    else if (energy < feh.k_off * floor)
        feh.gate_open = false;

    if (feh.gate_open == true)
        return true;

    feh.history[feh.idx++] = energy;
    //debug_uart_dma_float_buffer(&energy, 1, 200);
    if (feh.idx == feh.len)
        feh.idx = 0;
    return false;
}

inline float dsp_get_pitch_error_cents(float32_t *signal, int len)
{

    float detected_freq = mpm_get_pitch_f32(
        signal,
        len,
        ADC_SAMPLE_RATE,
        MPM_LAG_STOP_SEARCH,
        MPM_CLARITY_RATIO,
        MPM_PEAK_THRESHOLD);

    float target_freq = get_target_freq(detected_freq);
    float error_cents = get_error_in_cents(detected_freq, target_freq);
    target = target_freq;
    detected = detected_freq;

    // debug_uart_dma_float_buffer(&detected_freq,1, 50);

    return error_cents;
}

//============================================================================
// Private
//============================================================================
static inline float get_min(float32_t *values, int len)
{
    float min = FLT_MAX;
    for (int i = 0; i < len; i++)
        if (values[i] < min)
            min = values[i];
    return min;
}

static inline float get_percentile(float32_t *values, int len, float percentile)
{

    float32_t temp[len];
    for (int i = 0; i < len; i++)
        temp[i] = values[i];

    for (int i = 0; i < len - 1; i++)
        for (int j = 0; j < len - i - 1; j++)
            if (temp[j] > temp[j + 1])
            {
                float32_t swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }

    if (percentile < 0.0f)
        percentile = 0.0f;
    if (percentile > 1.0f)
        percentile = 1.0f;

    int index = (int)(percentile * (len - 1));
    return temp[index];
}
