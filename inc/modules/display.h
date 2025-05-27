#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

void display_init(void);

void display_clear_screen(void);
void display_print_string(const char *str);

void display_selection_screen(uint8_t index);
void display_metronome_screen(int bpm);
void display_tone_screen(int tone);

void display_pitch_indicator(const char *string, int error);
void display_clear_pitch_indicator(const char *string);

#endif // DISPLAY_H