#ifndef TUNING_LOGIC_H
#define TUNING_LOGIC_H



float get_target_freq(float detected_freq);
float get_error_in_cents(float detected_freq, float target_freq);

#endif /*TUNING_LOGIC_H*/