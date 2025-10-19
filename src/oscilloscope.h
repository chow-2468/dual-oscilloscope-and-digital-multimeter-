#ifndef _OSCILLOSCOPE_H
#define _OSCILLOSCOPE_H

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"



// adc
#define ADC1_DR_ADDRESS ((uint32_t)0x4001204C)

// timer
#define  MODE_1000ms_DIV 10
#define  MODE_500ms_DIV 5
#define  MODE_200ms_DIV 2
#define  MODE_100ms_DIV 1


void adc_timer(uint8_t);
void oscilloscope_init(volatile uint16_t*,uint16_t);

void create_linear_snapshot(volatile uint16_t* , uint16_t* , uint32_t );

void clear_buffer(uint32_t*, uint16_t);




#endif
