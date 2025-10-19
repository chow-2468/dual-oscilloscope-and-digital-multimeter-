/* Includes */
#include "touchScreen.h"

void SysTick_Init(void) {
    /* Disable SysTick */
    SysTick->CTRL = 0;

    /* Set reload register to generate 1ms interrupts */
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;

    /* Set priority for SysTick IRQ */
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    /* Reset counter value */
    SysTick->VAL = 0;

    /* Enable SysTick with processor clock, no interrupt */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(int ms) {
    /* Make sure SysTick is initialized */
    if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0) {
        SysTick_Init();
    }

    /* Wait for the specified number of milliseconds */
    for (int i = 0; i < ms; i++) {
        /* Clear current value */
        SysTick->VAL = 0;

        /* Wait until COUNTFLAG is set */
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) {
            /* Do nothing */
        }
    }
}

int8_t OSCbutton, DMMbutton;

void menuButtonInit(void){
	TM_ILI9341_Button_DeleteAll();
	TM_ILI9341_Button_t button;

	//oscilloscope button
	// Button configuration
	button.x = 5;
	button.y = 70;
	button.width = 150;
	button.height = 150;

	// Set the flags to prevent drawing text or border
	button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;

	//Add button
	OSCbutton = TM_ILI9341_Button_Add(&button);

	//DMM button
    button.x = 5 + 150 + 10;
	button.y = 70;
	button.width = 150;
	button.height = 150;
	// Set the flags to prevent drawing text or border
	button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
	//Add button
	DMMbutton = TM_ILI9341_Button_Add(&button);

	menuButtonEN();

}

void menuButtonEN(void){
	TM_ILI9341_Button_Enable(OSCbutton);
	TM_ILI9341_Button_Enable(DMMbutton);
}

int menuTS(void) {
    int8_t buttonPressed;

    if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {
//    	TM_ILI9341_Fill(ILI9341_COLOR_BLUE); //test
    	int mode = 0; // No button pressed
        buttonPressed = TM_ILI9341_Button_Touch(&touchData);
        if (buttonPressed == OSCbutton) {
            mode = 1; // Oscilloscope
        } else if (buttonPressed == DMMbutton) {
            mode = 2; // DMM
        }
        else
        	mode = -1;
        return mode;
    }
    else
    	return -1;

}


int8_t OSC_CHANGE_DIVISION_button, OSC_STORE_button, OSC_RESTORE_button;
int8_t OSC_MENU_button, OSC_CH1_button, OSC_CH2_button, OSC_SHOW_MATRICES_button;
void oscButtonInit(void) {
    TM_ILI9341_Button_DeleteAll();
    TM_ILI9341_Button_t button;

    // Bottom Buttons
    int x_start_bottom = 0;
    int y_start_bottom = 188;
    int x_end_bottom = 240;
    int y_end_bottom = 220;
    int num_buttons_bottom = 3;
    int spacing_bottom = 7;
    int button_width_bottom = (x_end_bottom - x_start_bottom - (num_buttons_bottom - 1) * spacing_bottom) / num_buttons_bottom;
    int button_height_bottom = y_end_bottom - y_start_bottom;

    button.height = button_height_bottom;
    button.width = button_width_bottom;
    button.y = y_start_bottom;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;

    // Change Division button
    button.x = x_start_bottom;
    OSC_CHANGE_DIVISION_button = TM_ILI9341_Button_Add(&button);

    // Store button
    button.x = x_start_bottom + (button_width_bottom + spacing_bottom);
    OSC_STORE_button = TM_ILI9341_Button_Add(&button);

    // Restore button
    button.x = x_start_bottom + 2 * (button_width_bottom + spacing_bottom);
    OSC_RESTORE_button = TM_ILI9341_Button_Add(&button);

    // Right Menu Bar Buttons
    int x_start_right = 245;
    int y_start_right = 20;
    int x_end_right = 315;
    int y_end_right = 223;
    int num_buttons_right = 4;
    int spacing_right = 7;
    int button_width_right = x_end_right - x_start_right;
    int button_height_right = (y_end_right - y_start_right - (num_buttons_right - 1) * spacing_right) / num_buttons_right;

    button.width = button_width_right;
    button.height = button_height_right;
    button.x = x_start_right;

    // Menu button
    button.y = y_start_right;
    OSC_MENU_button = TM_ILI9341_Button_Add(&button);

    // CH 1 button
    button.y = y_start_right + (button_height_right + spacing_right);
    OSC_CH1_button = TM_ILI9341_Button_Add(&button);

    // CH 2 button
    button.y = y_start_right + 2 * (button_height_right + spacing_right);
    OSC_CH2_button = TM_ILI9341_Button_Add(&button);

    // Show Matrices button
    button.y = y_start_right + 3 * (button_height_right + spacing_right);
    OSC_SHOW_MATRICES_button = TM_ILI9341_Button_Add(&button);

    // Call the function to enable the buttons
    oscButtonEN();
}

void oscButtonEN(void) {
    TM_ILI9341_Button_Enable(OSC_CHANGE_DIVISION_button);
    TM_ILI9341_Button_Enable(OSC_STORE_button);
    TM_ILI9341_Button_Enable(OSC_RESTORE_button);
    TM_ILI9341_Button_Enable(OSC_MENU_button);
    TM_ILI9341_Button_Enable(OSC_CH1_button);
    TM_ILI9341_Button_Enable(OSC_CH2_button);
    TM_ILI9341_Button_Enable(OSC_SHOW_MATRICES_button);
}

int oscTS(void) {
    int8_t buttonPressed;
    delay_ms(30);
    if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {

        int mode = -1;
        buttonPressed = TM_ILI9341_Button_Touch(&touchData);

        if (buttonPressed == OSC_MENU_button) {
            mode = 0; // Menu button pressed
        } else if (buttonPressed == OSC_CH1_button) {
            mode = 1; // CH 1 button pressed
        } else if (buttonPressed == OSC_CH2_button) {
            mode = 2; // CH 2 button pressed
        } else if (buttonPressed == OSC_SHOW_MATRICES_button) {
            mode = 3; // Show Matrices button pressed
        } else if (buttonPressed == OSC_CHANGE_DIVISION_button) {
            mode = 4; // Change Division button pressed
        } else if (buttonPressed == OSC_STORE_button) {
            mode = 5; // Store button pressed
        } else if (buttonPressed == OSC_RESTORE_button) {
            mode = 6; // Restore button pressed
        }

        return mode;
    }

    return -1; // No button pressed
}

int8_t DMM_MENU_button, DMM_TEMP_button, DMM_VOLT_button, DMM_CAP_button, DMM_RES_button, DMM_CUR_button, DMM_UP_button, DMM_DOWN_button;

void dmmButtonInit(void) {
	TM_ILI9341_Button_DeleteAll();
    TM_ILI9341_Button_t button;
    // Menu button
    button.x = 5;
    button.y = 20;
    button.width = 50;
    button.height = 105;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_MENU_button = TM_ILI9341_Button_Add(&button);

    // Temperature button
    button.x = 5;
    button.y = 130;
    button.width = 98;
    button.height = 50;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_TEMP_button = TM_ILI9341_Button_Add(&button);

    // Capacitance (Cap) button
    button.x = 5 + 98 + 5;
    button.y = 130;
    button.width = 98;
    button.height = 50;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_CAP_button = TM_ILI9341_Button_Add(&button);

    // Voltage button
    button.x = 5;
    button.y = 185;
    button.width = 98;
    button.height = 50;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_VOLT_button = TM_ILI9341_Button_Add(&button);

    // Resistance (Res) button
    button.x = 5 + 98 + 5;
    button.y = 185;
    button.width = 98;
    button.height = 50;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_RES_button = TM_ILI9341_Button_Add(&button);

    // Current (Cur) button
    button.x = 5 + (98 + 5) * 2;
    button.y = 185;
    button.width = 98;
    button.height = 50;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_CUR_button = TM_ILI9341_Button_Add(&button);

    // Up button
    button.x = 5 + (98 + 5) * 2;
    button.y = 50 - 30;
    button.width = 98;
    button.height = 78;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_UP_button = TM_ILI9341_Button_Add(&button);

    // Down button
    button.x = 5 + (98 + 5) * 2;
    button.y = 145 - 42;
    button.width = 98;
    button.height = 77;
    button.flags = TM_BUTTON_FLAG_NOLABEL | TM_BUTTON_FLAG_NOBORDER;
    DMM_DOWN_button = TM_ILI9341_Button_Add(&button);

    dmmButtonEN();
//    TM_ILI9341_Button_DrawAll();
}

void dmmButtonEN(void){
    TM_ILI9341_Button_Enable(DMM_MENU_button);
    TM_ILI9341_Button_Enable(DMM_TEMP_button);
    TM_ILI9341_Button_Enable(DMM_VOLT_button);
    TM_ILI9341_Button_Enable(DMM_CAP_button);
    TM_ILI9341_Button_Enable(DMM_RES_button);
    TM_ILI9341_Button_Enable(DMM_CUR_button);
    TM_ILI9341_Button_Enable(DMM_UP_button);
    TM_ILI9341_Button_Enable(DMM_DOWN_button);
}

int dmmTS(void) {
	int8_t buttonPressed;
	delay_ms(150);

    if (TM_STMPE811_ReadTouch(&touchData) == TM_STMPE811_State_Pressed) {

    	int mode = -1;
        buttonPressed = TM_ILI9341_Button_Touch(&touchData);

		if (buttonPressed == DMM_MENU_button) {
			mode = 0; // Menu
		} else if (buttonPressed == DMM_TEMP_button) {
			mode = 1; // Temperature
		} else if (buttonPressed == DMM_VOLT_button) {
			mode = 2; // Voltage
		} else if (buttonPressed == DMM_CAP_button) {
			mode = 3; // Capacitance
		} else if (buttonPressed == DMM_RES_button) {
			mode = 4; // Resistance
		} else if (buttonPressed == DMM_CUR_button) {
			mode = 5; // Current
		} else if (buttonPressed == DMM_UP_button) {
			mode = 6; // Up button
		} else if (buttonPressed == DMM_DOWN_button) {
			mode = 7; // Down button
		}


        return mode;
    }

    return -1; // No button pressed
}
