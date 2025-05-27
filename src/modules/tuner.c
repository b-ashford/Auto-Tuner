#include "modules/tuner.h"
#include "modules/signal.h"
#include "modules/motor.h"
#include "modules/display.h"
#include <math.h>

// frequency constants
#define E2_FREQ   82.41f
#define A2_FREQ  110.00f
#define D3_FREQ  146.83f
#define G3_FREQ  196.00f
#define B3_FREQ  246.94f
#define E4_FREQ  329.63f

static float32_t target_freqs[NUM_STRINGS];
static const char *string_names[NUM_STRINGS] = { "E2", "A2", "D3", "G3", "B3", "E4" };
static uint8_t string_index;
static int correct_count;
static void (*state_fn)(void);

static void state_get_pitch(void);
static void state_string_pitch(int up_lim, int low_lim, float32_t target);

void tuner_init(void) {
    target_freqs[0] = E2_FREQ;
    target_freqs[1] = A2_FREQ;
    target_freqs[2] = D3_FREQ;
    target_freqs[3] = G3_FREQ;
    target_freqs[4] = B3_FREQ;
    target_freqs[5] = E4_FREQ;

    string_index = 0;
    correct_count = 0;
    state_fn = state_get_pitch;
}

void tuner_run(void) {
    state_fn();
}

static void state_get_pitch(void) {
    // define bounds for each string
    static const int up_lims[NUM_STRINGS]   = {100,140,190,230,320,350};
    static const int low_lims[NUM_STRINGS]  = { 55, 85,115,155,205,305};

    state_string_pitch(
        up_lims[string_index],
        low_lims[string_index],
        target_freqs[string_index]
    );
}

static void state_string_pitch(int up_lim, int low_lim, float32_t target) {
    // move to next string when tuned correct MIN_CORRECT times
    if (correct_count >= MIN_CORRECT) {
        display_clear_screen();
        display_print_string("Next");
        string_index = (string_index + 1) % NUM_STRINGS;
        correct_count = 0;
        return;
    }
    // collect initial table
    while (!init_table(up_lim, low_lim, target));

    float32_t freq = 0;
    get_frequency(guitar_signal, target, &freq);

    if (freq > low_lim && freq < up_lim) {
        // add to buffer and compute error
        // pitch_table is updated inside init_table
        float32_t min_freq = init_table(up_lim, low_lim, target) ? 0 : 0; // buffer ready
        // actually fetch min from signal module
        extern float get_min_in_buffer(void);
        min_freq = get_min_in_buffer();
        int error = (int)get_error_in_cents(min_freq, target);
        display_pitch_indicator(string_names[string_index], error);

        if (motor_wait == 0) {
            if (error > 40)       tune_down_fast();
            else if (error > 7)   tune_down_fine();
            else if (error < -40)  tune_up_fast();
            else if (error < -7)   tune_up_fine();
            else                   correct_count++;
        }
    } else {
        display_clear_pitch_indicator(string_names[string_index]);
    }
    toggle_motor_wait();
}