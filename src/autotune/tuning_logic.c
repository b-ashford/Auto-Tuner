
#include "tuning_logic.h"
#include "guitar_config.h"
#include <math.h>

float get_error_in_cents(float detected_freq, float target_freq)
{
    if (detected_freq <= 0.0f || target_freq <= 0.0f) 
        return 0.0f;
    return 1200.0f * log2f(detected_freq / target_freq);
}