#ifndef AUTOTUNE_MUSIC_H
#define AUTOTUNE_MUSIC_H

#include <math.h>

#define NUM_STRINGS 6

typedef enum
{
    NOTE_E2,
    NOTE_A2,
    NOTE_D3,
    NOTE_G3,
    NOTE_B3,
    NOTE_E4,
    NOTE_COUNT
} string_t;

static const float note_freq[NOTE_COUNT] = {
    [NOTE_E2] = 82.41,
    [NOTE_A2] = 110.00,
    [NOTE_D3] = 146.83,
    [NOTE_G3] = 196.00,
    [NOTE_B3] = 246.94,
    [NOTE_E4] = 329.63};

static const char *const guitar_string_names[NOTE_COUNT] = {
    "E2",
    "A2",
    "D3",
    "G3",
    "B3",
    "E4"};

float get_error_in_cents(float detected_freq, float target_freq);
float get_target_freq(float detected_freq);

#endif /* AUTOTUNE_MUSIC_H */
