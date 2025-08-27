#ifndef PITCH_H
#define PITCH_H


#include <stdbool.h>
#include "arm_math.h"


float get_pitch_error_cents(float32_t *signal, int len);


#endif /* PITCH_H */