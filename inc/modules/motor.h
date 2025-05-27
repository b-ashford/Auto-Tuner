#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void motor_init(void);
void tune_up_fine(void);
void tune_up_fast(void);
void tune_down_fine(void);
void tune_down_fast(void);
void toggle_motor_wait(void);

#endif // MOTOR_H