#include "oscilloscope.h"


void adc_timer(uint8_t div){ // can change
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	uint16_t period = (25*div)-1;
	TIM_TimeBaseInitTypeDef TIM3_InitStructure;
	TIM3_InitStructure.TIM_Prescaler = 8999;
	TIM3_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM3_InitStructure.TIM_Period = period;
	TIM3_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_InitStructure.TIM_RepetitionCounter = 0;

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_TimeBaseInit(TIM3,&TIM3_InitStructure);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
}

void oscilloscope_init(volatile uint16_t* BUFFER_ADDRESS, uint16_t SIZE) {


    // --- 1. Enable Clocks ---
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // --- 2. Configure GPIOs for ONLY the active channels ---
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStruct);


    // --- 3. ADC Common Configuration ---
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2; // Use a safe prescaler
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);

    // --- 4. ADC1 Specific Configuration ---
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode =ENABLE;// if multichannel then enable
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStruct.ADC_NbrOfConversion = 2;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_15Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_15Cycles);


    // --- 6. Configure DMA ---
    DMA_InitTypeDef DMA_InitStruct;
    DMA_DeInit(DMA2_Stream0); // Ensure a clean state before re-configuring
    DMA_InitStruct.DMA_Channel = DMA_Channel_0;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)BUFFER_ADDRESS;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_BufferSize = SIZE;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode =DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStruct);

    // --- 7. Link ADC to DMA and Start ---
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    DMA_Cmd(DMA2_Stream0, ENABLE);
}

void clear_buffer(uint32_t *BUFFER_ADDRESS, uint16_t SIZE){
	for(int i=0;i<SIZE; i++)
		*(BUFFER_ADDRESS+i)=0;
}



// ori buffer, after arrange buffer
void create_linear_snapshot(volatile uint16_t* circular_buffer, uint16_t* linear_snapshot, uint32_t num_total_samples) {

    // 1. Find the starting read position (the oldest data)
    uint32_t read_index = (((480 - DMA_GetCurrDataCounter(DMA2_Stream0)) & 0xFFFE)+2) % 480;

    // 2. Loop through and copy the data, re-ordering it into the linear buffer
    for (uint32_t i = 0; i < num_total_samples; i++) {
        // Write sequentially into the linear buffer from index 0
        linear_snapshot[i] = circular_buffer[read_index];
        // Advance the read index, wrapping around the circular buffer if necessary
        read_index = (read_index + 1) % 480;
    }
}
