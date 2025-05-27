#include "modules/modes.h"
#include "modules/display.h"
#include "main.h"
#include "modules/signal.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
extern DAC_HandleTypeDef hdac1;

static bool in_selection = true;
static uint8_t select_index = 0; // 0: tone, 1: metronome

void mode_select_init(void) {
    in_selection = true;
    select_index = 0;
    display_selection_screen(select_index);
}

bool mode_is_selection(void) {
    return in_selection;
}

void handle_mode_selection(void) {
    bool up = HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin);
    bool down = HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin);
    if (up || down) {
        select_index ^= 1;
        display_selection_screen(select_index);
        HAL_Delay(100);
    }
    if (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin)) {
        in_selection = false;
        if (select_index == 0) tone_gen_mode(); else metronome_mode();
        mode_select_init();
    }
}

void metronome_mode(void) {
    HAL_TIM_Base_Start_IT(&htim2);
    int bpm = 100;
    display_metronome_screen(bpm);
    while (!HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin)) {
        if (HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin)) {
            bpm++;
            display_metronome_screen(bpm);
            HAL_Delay(100);
        }
        if (HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin)) {
            bpm--;
            display_metronome_screen(bpm);
            HAL_Delay(100);
        }
        uint32_t prescale = 10000 * 60 / bpm;
        __HAL_TIM_SET_PRESCALER(&htim2, prescale);
    }
    HAL_TIM_Base_Stop_IT(&htim2);
}

void tone_gen_mode(void) {
    display_tone_screen(100);
    HAL_TIM_Base_Start(&htim7);
    uint32_t sineval[100];
    extern void get_sineval(uint32_t *);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_1, sineval, 100, DAC_ALIGN_12B_R);
    while (!HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin)) {
        get_sineval(sineval);
    }
    HAL_DAC_Stop_DMA(&hdac1, DAC1_CHANNEL_1);
    HAL_TIM_Base_Stop(&htim7);
}