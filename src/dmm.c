#include "dmm.h"
#include "dht22.h"
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"


void dmm_init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


	//Initialize PC12 as output for buzzer
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


}

void dmm_TIM2_Init(void) {
  uint16_t PrescalerValue = 0;

  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;

  //TIM2 clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  //Compute the prescaler value
  PrescalerValue = (uint16_t) (90000000 / 1000000) - 1;

  //Time base configuration
  TIM_TimeBaseStructure.TIM_Period = 65535 - 1;      //in uSecs
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_Cmd(TIM2, ENABLE);
}

void dht22_interrupt(void){ //lcd refresh rate
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

	TIM_TimeBaseInitTypeDef TIM5_InitStructure;

	TIM5_InitStructure.TIM_Prescaler = 8999;
	TIM5_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM5_InitStructure.TIM_Period = 24999;
	TIM5_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM5_InitStructure.TIM_RepetitionCounter = 0;

	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_TimeBaseInit(TIM5,&TIM5_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t read_adc(uint16_t channel){
	if (channel-1)
		ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_15Cycles);
	else
		ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);
	ADC_SoftwareStartConv(ADC2);
	while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET){};
	return ADC_GetConversionValue(ADC2);
}


float dmm_value(int mode,volatile uint16_t* buffer,volatile uint8_t* get_temp,volatile float *temp_value){
	float value = 0;


	uint16_t adc_volt_1 ;
	adc_volt_1 = buffer[0];

	uint16_t adc_volt_2 ;
	adc_volt_2 = buffer[1];

	float volt_1 = (adc_volt_1 * 3.3f) / 4095.0f;
	float volt_2 = (adc_volt_2 * 3.3f) / 4095.0f;
	float Vg = volt_1 - volt_2;
	int R1 = 10000;
	int R2 = 10000;
	int R3 = 10000;
	int Rcur = 10000;
	float Cap1 = 0.00001;


	switch (mode){
		case 1:  //read value of temperature from sensor
			if(*get_temp){
				*get_temp=0;
				DHT22_Read();
				*temp_value = DHT22getTemperature();
			}
			value = *temp_value;
			break;
		case 2:  //calculate value of voltage
			value = Vg;
			break;
		case 3:  //calculate value of capacitance
			value = (Vg*Cap1) / (3.3-Vg);
			break;
		case 4:  //calculate value of resistance
			//value = ((3.3*R2 - (R1+R2)*Vg) * R3) / (3.3*R1 + (R1+R2)*Vg);
			value = (R3*(3.3*R2 - Vg*R1 - Vg*R2)) / (Vg*R1 + Vg*R2 + 3.3*R1);
			break;
		case 5:  //calculate value of current
			value = Vg / Rcur;
			break;
		default:
			value = 0;
			break;
	}
	return value;
}
