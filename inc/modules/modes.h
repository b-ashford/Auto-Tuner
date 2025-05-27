#ifndef MODES_H
#define MODES_H

#include <stdint.h>
#include <stdbool.h>

void mode_select_init(void);
bool mode_is_selection(void);
void handle_mode_selection(void);
void metronome_mode(void);
void tone_gen_mode(void);

#endif // MODES_H