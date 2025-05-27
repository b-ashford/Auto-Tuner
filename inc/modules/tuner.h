#ifndef TUNER_H
#define TUNER_H

#include <stdint.h>

#define NUM_STRINGS     6
#define MIN_CORRECT     3

void tuner_init(void);
void tuner_run(void);

#endif // TUNER_H