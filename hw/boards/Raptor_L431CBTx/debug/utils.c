
#include "debug/utils.h"

#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"

static inline void copy_buffer_f32(float32_t *signal, int len);
static ALIGN4 float32_t debug_tx[SIGNAL_BUFF_LEN];

void init_debug(void)
{
    SWO_Init();
    ITM_Init();
}

void SWO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3; // SWO pin
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SWJ; // SWJ alternate function
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// Initialize ITM
void ITM_Init(void)
{
    // Enable ITM
    ITM->LAR = 0xC5ACCE55; // Unlock ITM
    ITM->TER = 0x00000003; // Enable ports 0 and 1
    ITM->TCR = 0x0001000D; // Enable ITM with sync packets

    // Configure TPIU for SWO
    TPI->SPPR = 0x00000002;               // Use async mode
    TPI->ACPR = (80000000 / 2000000) - 1; // Set SWO speed (CPU_FREQ / SWO_FREQ - 1)
}

HAL_StatusTypeDef debug_uart_dma_float_buffer(float *buff, int len, uint32_t wait_ms)
{
    copy_buffer_f32(buff, len);
    static uint32_t last_send_time = 0;
    uint32_t current_time = HAL_GetTick();

    if (current_time - last_send_time < wait_ms)
        return HAL_BUSY;
    if (huart1.gState != HAL_UART_STATE_READY)
        return HAL_BUSY;

    HAL_StatusTypeDef result = HAL_UART_Transmit_DMA(
        &huart1,
        (uint8_t *)debug_tx,
        len * sizeof(float));

    if (result == HAL_OK)
        last_send_time = current_time;

    return result;
}

HAL_StatusTypeDef debug_uart_dma_uint16_buffer(uint16_t *buff, int len, uint32_t wait_ms)
{
    static uint32_t last_send_time = 0;
    uint32_t current_time = HAL_GetTick();

    if (current_time - last_send_time < wait_ms)
        return HAL_BUSY;
    if (huart1.gState != HAL_UART_STATE_READY)
        return HAL_BUSY;

    HAL_StatusTypeDef result = HAL_UART_Transmit_DMA(
        &huart1,
        (uint8_t *)buff,
        len * sizeof(uint16_t));
    if (result == HAL_OK)
        last_send_time = current_time;

    return result;
}

HAL_StatusTypeDef debug_uart_dma_print_string(const char *str)
{
    if (huart1.gState != HAL_UART_STATE_READY)
        return HAL_BUSY;
    return HAL_UART_Transmit_DMA(&huart1, (uint8_t *)str, strlen(str));
}

static inline void copy_buffer_f32(float32_t *signal, int len)
{
    if (len > SIGNAL_BUFF_LEN)
        len = SIGNAL_BUFF_LEN;
    for (int i = 0; i < len; i++)
        debug_tx[i] = signal[i];
}

inline void debug_dwt_enable(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

inline uint32_t debug_start_cycle_count(void)
{
    return DWT->CYCCNT;
}

inline float debug_end_cycle_us(uint32_t start_cycles, float clock_mhz)
{
    uint32_t end_cycles = DWT->CYCCNT;
    uint32_t cycles = end_cycles - start_cycles;
    return (float)cycles / clock_mhz;
}