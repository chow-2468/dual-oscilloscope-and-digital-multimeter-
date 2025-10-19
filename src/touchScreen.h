/* Includes */
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"


#include <string.h>
#include <stdint.h>
#include <math.h>

/* Private function prototypes */
void SysTick_Init(void);
void delay_ms(int);
void menuButtonInit(void);
void menuButtonEN(void);
int menuTS(void);
void oscButtonInit(void);
void oscButtonEN(void);
int oscTS(void);
void dmmButtonInit(void);
void dmmButtonEN(void);
int dmmTS(void);



/* Global variables for touch */
TM_STMPE811_TouchData touchData;
