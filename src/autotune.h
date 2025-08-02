#ifndef AUTOTUNE_H
#define AUTOTUNE_H

#include <stdint.h>

#define DSP_BUFF_LEN 1024
#define ADC_BUFF_LEN  2 * DSP_BUFF_LEN

extern uint16_t adc_buffer[ADC_BUFF_LEN];

void start_autotune_mode(void); 
void process_adc(void);

#endif /* AUTOTUNE_H */
 