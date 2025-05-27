#include "core/init.h"
#include "modules/tuner.h"
#include "modules/modes.h"
#include "modules/gyro.h"
#include "modules/display.h"

int main(void) {
    HAL_Init();
    setup_system();           // SystemClock_Config
    init_peripherals();       // MX_*_Init calls

    gyro_init();
    display_init();
    signal_init();
    adc_start();
    motor_init();

    tuner_init();
    mode_select_init();

    for (;;) {
        if (mode_is_selection()) handle_mode_selection();
        else tuner_run();
    }
}

