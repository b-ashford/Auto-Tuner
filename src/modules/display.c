#include "modules/display.h"
#include "ssd1306.h"
#include "modules/oled_print.h"
#include <string.h>

void display_init(void) {
    ssd1306_Init();
}

void display_clear_screen(void) {
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
}

void display_print_string(const char *str) {
    oled_print_string((char*)str);
}

void display_selection_screen(uint8_t index) {
    display_clear_screen();
    const char *items[] = { "Tone", "Metronome" };
    oled_selection_screen();
    // highlight
    if (index == 0)
        ssd1306_DrawRectangle(0, 36, 128, 58, Black);
    else
        ssd1306_DrawRectangle(0, 8, 128, 30, Black);
    ssd1306_UpdateScreen();
}

void display_metronome_screen(int bpm) {
    display_clear_screen();
    oled_timing_screen(bpm);
    ssd1306_UpdateScreen();
}

void display_tone_screen(int tone) {
    display_clear_screen();
    oled_tone_screen(tone);
    ssd1306_UpdateScreen();
}

void display_pitch_indicator(const char *string, int error) {
    oled_print_pitch_indicator_screen((char*)string, error);
}

void display_clear_pitch_indicator(const char *string) {
    oled_clear_pitch_indicator_tick((char*)string);
}