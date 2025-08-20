
#include "tuning_logic.h"
#include "guitar_config.h"
#include <math.h>



inline float get_error_in_cents(float detected_freq, float target_freq)
{
    if (detected_freq <= 0.0f || target_freq <= 0.0f)
        return 0.0f;
    return 1200.0f * log2f(detected_freq / target_freq);
}

inline float get_target_freq(float detected_freq)
{
    if (detected_freq <= 0.0f)
        return note_freq[0]; // fallback

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