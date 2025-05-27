#include "modules/gyro.h"
#include "drivers/TJ_MPU6050.h"
#include "ssd1306.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;
static ScaledData_Def accel, gyro;
static int screen_flipped = 0;

void gyro_init(void) {
    MPU6050_Init(&hi2c1);
    MPU6050_Config(&(MPU_ConfigTypeDef){
        .Accel_Full_Scale = AFS_SEL_2g,
        .CONFIG_DLPF = Internal_8MHz,
        .ClockSource = DLPF_184A_188G_Hz,
        .Gyro_Full_Scale = FS_SEL_500,
        .Sleep_Mode_Bit = 0
    });
    HAL_TIM_Base_Start_IT(&htim16);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM16) {
        MPU6050_Get_Accel_Scale(&accel);
        MPU6050_Get_Gyro_Scale(&gyro);
        if (accel.x > 0 && !screen_flipped) {
            ssd1306_Init1();
            screen_flipped = 1;
        } else if (accel.x < 0 && screen_flipped) {
            ssd1306_Init2();
            screen_flipped = 0;
        }
    }
}


