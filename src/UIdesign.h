/* Includes */
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "logo.h"
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_dma2d_graphic.h"

#include <stdint.h>
#include <math.h>

/* Colors */
#define ILI9341_COLOR_SLATE_BLUE    0x29B3
#define ILI9341_COLOR_DARK_BLUE     0x118B
#define ILI9341_COLOR_LIGHT_GREEN_CYAN 0x2DD1
#define ILI9341_COLOR_MOSS_GREEN    0x3C50
#define ILI9341_COLOR_DARK_CYAN 0x1089

/* Private function prototypes */
void display_splash_screen(void);
void drawMenuUI(void);
void drawGraphUI(void);
void drawAxis(void);
void drawOscilloscopeUI(void);
void drawRightMenuBar();
void drawDMMUI(void);
void drawDmmModesButtons(int);
void drawOmegaSymbol(int, int, int, uint16_t, uint16_t);
void drawDegreeCSymbol(int, int, int, uint16_t, uint16_t);
uint16_t interpolate_color(uint16_t, uint16_t, float);


