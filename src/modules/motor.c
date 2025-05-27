#include "modules/motor.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern volatile int motor_wait;

static const uint16_t PW_NEUTRAL      = 50;
static const uint16_t PW_UP_FINE      = 46;
static const uint16_t PW_UP_FAST      = 40;
static const uint16_t PW_DOWN_FINE    = 54;
static const uint16_t PW_DOWN_FAST    = 64;

void motor_init(void) {
    motor_wait = 0;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_NEUTRAL);
}

void tune_up_fine(void) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_UP_FINE);
    HAL_Delay(100);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_NEUTRAL);
}

void tune_up_fast(void) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_UP_FAST);
    HAL_Delay(200);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_NEUTRAL);
    HAL_Delay(100);
}

void tune_down_fine(void) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_DOWN_FINE);
    HAL_Delay(100);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_NEUTRAL);
}

void tune_down_fast(void) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_DOWN_FAST);
    HAL_Delay(200);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PW_NEUTRAL);
    HAL_Delay(100);
}

void toggle_motor_wait(void) {
    motor_wait = (motor_wait + 1) % 3;
}
