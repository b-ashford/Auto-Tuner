
#include "autotune/pitch.h"
#include "autotune/params.h"
#include "autotune/mpm.h"
#include "autotune/guitar.h"
#include "debug/utils.h"
#include <float.h>

static inline float get_error_in_cents(float detected_freq, float target_freq);
static inline float get_target_freq(float detected_freq);

float target = 0.0f;
float detected = 0.0f;


inline float get_pitch_error_cents(float32_t *signal, int len)
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
    
    float dpack[3] = {error_cents, detected_freq, target_freq};
    debug_uart_dma_float_buffer(dpack,3, 128);

    return error_cents;
}

//============================================================================
// Private
//============================================================================

static inline float get_error_in_cents(float detected_freq, float target_freq)
{
    if (detected_freq <= 0.0f || target_freq <= 0.0f)
        return 0.0f;
    return 1200.0f * log2f(detected_freq / target_freq);
}

static inline float get_target_freq(float detected_freq)
{
    if (detected_freq <= 0.0f)
        return note_freq[0]; 

    int string_index = 0;
    float min_error = fabsf(
        get_error_in_cents(detected_freq, note_freq[NOTE_E2]));

    for (int i = 1; i < NOTE_COUNT; i++)
    {
        float error = fabsf(get_error_in_cents(detected_freq, note_freq[i]));
        if (error < min_error)
        {
            min_error = error;
            string_index = i;
        }
    }
    return note_freq[string_index];
}