#ifndef __DMM_H
#define __DMM_H

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"


void dmm_init(void);
void dmm_TIM2_Init(void);
float dmm_value(int, volatile uint16_t*,volatile uint8_t*,volatile float*);
void dht22_interrupt(void);

#endif
