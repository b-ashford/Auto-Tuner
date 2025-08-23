#ifndef AUTOTUNE_H
#define AUTOTUNE_H

#include "arm_math.h"
#include <stdint.h>
enum
{
    FIRST_HALF,
    SEDOND_HALF
};
void autotune_mode_init(void);
void autotune_mode_run(void);
void autotune_mode(int buffer_section);

#endif /* AUTOTUNE_H */
