// DHT22 Sensor function header file

/* Define to prevent recursive inclusion -----------------------------------*/
#ifndef __DHT22_H
#define __DHT22_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ----------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"

/* defines -----------------------------------------------------------------*/
#define DHT22_DATA_PIN  GPIO_Pin_11
#define DHT22_GPIO		GPIOC
#define DHT22_TIM		TIM2

/* variables ---------------------------------------------------------------*/
uint8_t DHT22data[6];

/* functions prototypes ----------------------------------------------------*/
void DHT22pinIn(void);
void DHT22pinOut(void);
void DHT22_Init(void);
void DHT22_Read(void);
float DHT22getTemperature(void);
float DHT22getHumidity(void);
float convertCtoF(float cTemperature);

#ifdef __cplusplus
}
#endif

#endif

