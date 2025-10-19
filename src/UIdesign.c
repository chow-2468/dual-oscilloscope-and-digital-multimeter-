/* Includes */
#include "UIdesign.h"


void display_splash_screen(void) {
    uint16_t x, y;
    uint32_t pixel_index;

    // The outer loop iterates through each row, from top (y=0) to bottom (y=319).
    for (y = 0; y < 240; y++) {
        for (x = 0; x < 320; x++) {
            pixel_index = (y * 320) + x;

            // Get the color for the current pixel from your 1D image array.
            uint16_t pixel_color = logo[pixel_index];

            // Draw that single pixel at its (x, y) coordinate on the screen.
            TM_ILI9341_DrawPixel(x, y, pixel_color);
        }
    }
}


void drawMenuUI() {
	//background color
    for (int y = 0; y < 240; y++) {
        float ratio = (float)y / 240;
        uint16_t gradient_color = interpolate_color(ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE, ratio);
        TM_ILI9341_DrawLine(0, y, 320, y, gradient_color);
    }

    // Main Menu Title
    TM_ILI9341_Puts(10, 10, "2-in-1 Measurement", &TM_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    TM_ILI9341_Puts(75, 35, "Instrument", &TM_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);

    // Button dimensions
    int button_width = 150;
    int button_height = 150;
    int button_padding = 10;

    /* Left Button: Oscilloscope */
    int osc_x = 5;
    int osc_y = 70;

    TM_ILI9341_DrawFilledRectangle(osc_x, osc_y, osc_x + button_width, osc_y + button_height, ILI9341_COLOR_DARK_BLUE);
    TM_ILI9341_DrawRectangle(osc_x, osc_y, osc_x + button_width, osc_y + button_height, ILI9341_COLOR_WHITE);

    // Oscilloscope button icon
    int graph_x = osc_x + 15;
    int graph_y = osc_y + 15;
    int graph_width = button_width - 30;
    int graph_height = 80;

    // Draw oscilloscope graph background and grid
    TM_ILI9341_DrawFilledRectangle(graph_x, graph_y, graph_x + graph_width, graph_y + graph_height, ILI9341_COLOR_BLACK);
    for (int i = 0; i <= 4; i++) {
        TM_ILI9341_DrawLine(graph_x, graph_y + i * 20, graph_x + graph_width, graph_y + i * 20, ILI9341_COLOR_GRAY);
    }
    for (int i = 0; i <= 6; i++) {
        TM_ILI9341_DrawLine(graph_x + i * 20, graph_y, graph_x + i * 20, graph_y + graph_height, ILI9341_COLOR_GRAY);
    }

    // Draw a small square wave
    int square_y;
    int prev_y = -1; // Initialize to an invalid value
    int period_length = graph_width / 4; // 4 periods in the full width

    for (int i = 0; i < graph_width; i++) {
        // Determine the vertical position for the current point
        if ((i % period_length) < (period_length / 2)) {
            square_y = graph_y + graph_height / 4;
        } else {
            square_y = graph_y + graph_height / 4 * 3;
        }

        // Only draw lines after the first point
        if (prev_y != -1) {
            // Draw the horizontal line segment
            TM_ILI9341_DrawLine(graph_x + i - 1, prev_y, graph_x + i, prev_y, ILI9341_COLOR_ORANGE);
            // Draw the vertical line segment
            TM_ILI9341_DrawLine(graph_x + i, prev_y, graph_x + i, square_y, ILI9341_COLOR_ORANGE);
        }

        // Update the previous y-coordinate for the next iteration
        prev_y = square_y;
    }

    // Draw button text
    TM_ILI9341_Puts(osc_x + 10, osc_y + button_height - 40, "Oscilloscope", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_DARK_BLUE);

    /* Right Button: Digital Multimeter (DMM) */
    int dmm_x = osc_x + button_width + button_padding;
    int dmm_y = osc_y;

    TM_ILI9341_DrawFilledRectangle(dmm_x, dmm_y, dmm_x + button_width, dmm_y + button_height, ILI9341_COLOR_MOSS_GREEN);
    TM_ILI9341_DrawRectangle(dmm_x, dmm_y, dmm_x + button_width, dmm_y + button_height, ILI9341_COLOR_WHITE);

    // DMM button icon
    int dmm_display_x = dmm_x + 15;
    int dmm_display_y = dmm_y + 15;
    int dmm_display_width = button_width - 30;
    int dmm_display_height = 60;

    // Draw DMM display
    TM_ILI9341_DrawFilledRectangle(dmm_display_x, dmm_display_y, dmm_display_x + dmm_display_width, dmm_display_y + dmm_display_height, ILI9341_COLOR_BLACK);
    TM_ILI9341_DrawRectangle(dmm_display_x, dmm_display_y, dmm_display_x + dmm_display_width, dmm_display_y + dmm_display_height, ILI9341_COLOR_WHITE);
    TM_ILI9341_Puts(dmm_display_x + 10, dmm_display_y + 15, "0.257", &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
    TM_ILI9341_Puts(dmm_display_x + 100, dmm_display_y + 20, "V", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

    // Draw button text
    TM_ILI9341_Puts(dmm_x + 40, dmm_y + button_height - 65, "Digital", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
    TM_ILI9341_Puts(dmm_x + 20, dmm_y + button_height - 45, "Multimeter", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
    TM_ILI9341_Puts(dmm_x + 50, dmm_y + button_height - 25, "(DMM)", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
}

void drawGraphUI(){
	TM_ILI9341_DrawFilledRectangle(0, 20, 240, 180, ILI9341_COLOR_BLACK);
	drawAxis();
}

void drawAxis(){
	TM_ILI9341_DrawRectangle(0, 20, 240, 180, ILI9341_COLOR_WHITE);
    // grid lines, size = 40x40
    // horizontal
    for (int i = 1; i <= 3; i++) {
        TM_ILI9341_DrawLine(0, 20 + i * 40, 240, 20 + i * 40, ILI9341_COLOR_GRAY);
    }
    // vertical
    for (int i = 1; i <= 5; i++) {
        TM_ILI9341_DrawLine(0 + i * 40, 20, 0 + i * 40, 180, ILI9341_COLOR_GRAY);
    }
    // X-axis
    //TM_ILI9341_DrawFilledRectangle(0, 99, 240, 101, ILI9341_COLOR_WHITE);
    TM_ILI9341_DrawLine(0, 180, 240, 180, ILI9341_COLOR_WHITE);
    for (int i = 0; i <= 24; i++) {
            TM_ILI9341_DrawLine(i*10, 180-2, i*10, 180, ILI9341_COLOR_WHITE);
        }
    // Y-axis
    //TM_ILI9341_DrawFilledRectangle(119, 20, 121, 180, ILI9341_COLOR_WHITE);
    TM_ILI9341_DrawLine(120, 20, 120, 180, ILI9341_COLOR_WHITE);
    for (int i = 0; i <= 16; i++) {
                TM_ILI9341_DrawLine(118, 20+i*10, 122, 20+i*10, ILI9341_COLOR_WHITE);
            }
    //outline

}


void drawOscilloscopeUI() {
    // blue background
    TM_ILI9341_Fill(ILI9341_COLOR_DARK_BLUE);

    // oscilloscope background
    drawGraphUI();
    // top text
	TM_ILI9341_Puts(5, 5, "Oscilloscope", &TM_Font_7x10, ILI9341_COLOR_GRAY, ILI9341_COLOR_DARK_BLUE);
	TM_ILI9341_Puts(120-7, 5, "4V", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_DARK_BLUE);

	// right buttons;
	drawRightMenuBar(0,0);

    // bottom buttons
    int x_start = 0;
    int y_start = 188;
    int x_end = 240;
    int y_end = 220;
    int num_buttons = 3;
    int spacing = 7;
    char* labels1[] = {"Change", "Store", "Restore"};
    char* labels2[] = {"Division", "", ""};

    // Calculate the width and height of each button to ensure equal size
    int button_width = (x_end - x_start - (num_buttons - 1) * spacing) / num_buttons;
    int button_height = y_end - y_start;

    // Loop to draw the buttons
    for (int i = 0; i < num_buttons; i++) {
        int rect_x_start = x_start + i * (button_width + spacing);
        int rect_x_end = rect_x_start + button_width;

        TM_ILI9341_DrawFilledRectangle(rect_x_start, y_start, rect_x_end, y_end, ILI9341_COLOR_MOSS_GREEN);
        TM_ILI9341_DrawRectangle(rect_x_start, y_start, rect_x_end, y_end, ILI9341_COLOR_WHITE);

        int font_width;
        int font_height;

        if (i == 1) { // "Store" button
            font_width = 11;
            font_height = 18;
        } else { // "Change" and "Restore" buttons
            font_width = 7;
            font_height = 10;
        }

        // Center text horizontally and vertically
        int text_x1 = rect_x_start + (button_width - strlen(labels1[i]) * font_width) / 2;
        int text_y1;

        if (i == 0) {
            text_y1 = y_start + (button_height / 2) - 10;
        } else {
            text_y1 = y_start + (button_height - font_height) / 2;
        }

        if (i == 0) {
            TM_ILI9341_Puts(text_x1, text_y1, labels1[i], &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
            int text_x2 = rect_x_start + (button_width - strlen(labels2[i]) * 7) / 2;
            int text_y2 = y_start + (button_height / 2);
            TM_ILI9341_Puts(text_x2, text_y2, labels2[i], &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
        } else if (i == 1) {
            TM_ILI9341_Puts(text_x1, text_y1, labels1[i], &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
        } else {
            TM_ILI9341_Puts(text_x1, text_y1, labels1[i], &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
        }
    }


    // bottom text:
    TM_ILI9341_Puts(5, 228, "Selected:", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_DARK_BLUE);

}

void drawRightMenuBar(int ch1, int ch2) {
	TM_ILI9341_DrawFilledRectangle(241, 0, 320, 223, ILI9341_COLOR_DARK_BLUE);
    char *button_texts1[] = {"Menu", "CH 1", "CH 2", "Show"};
    char *button_texts2[] = {NULL, NULL, NULL, "Matrices"};
    int num_buttons = 4;
    int x_start = 245;
    int y_start_range = 20;
    int x_end = 315;
    int y_end_range = 223;
    int gap = 7;

    uint16_t colorCH1 = ch1 ? ILI9341_COLOR_YELLOW:ILI9341_COLOR_SLATE_BLUE; // button colour
	uint16_t colorTxtCH1 = ch1 ? ILI9341_COLOR_BLACK :ILI9341_COLOR_WHITE;
    uint16_t colorCH2 = ch2 ? ILI9341_COLOR_ORANGE:ILI9341_COLOR_SLATE_BLUE;

    // Calculate button dimensions
    int button_width = x_end - x_start;
    int button_height = (y_end_range - y_start_range - (num_buttons - 1) * gap) / num_buttons;


    int y_pos;

    for (int i = 0; i < num_buttons; i++) {
    	uint16_t TxTcolor = ILI9341_COLOR_WHITE;
		uint16_t BGcolor = ILI9341_COLOR_SLATE_BLUE;
        y_pos = y_start_range + i * (button_height + gap);


        if (i < 3) {
        	switch (i){
        	case 1: //ch1
        		TxTcolor = colorTxtCH1;
        		BGcolor = colorCH1;
        		break;

        	case 2:
        		BGcolor = colorCH2;
        		break;
        	default:
        		break;

        	}
        	TM_ILI9341_DrawFilledRectangle(x_start, y_pos, x_end, y_pos + button_height, BGcolor);
        	TM_ILI9341_DrawRectangle(x_start, y_pos, x_end, y_pos + button_height, ILI9341_COLOR_WHITE);
            // "Menu", "CH 1", "CH 2" buttons with a larger font
            int text_x = x_start + (button_width / 2) - (strlen(button_texts1[i]) * 11 / 2);
            int text_y = y_pos + (button_height - 18) / 2;
            TM_ILI9341_Puts(text_x, text_y, button_texts1[i], &TM_Font_11x18, TxTcolor, BGcolor);
        } else {
            // "Show Matrices" with two lines and smaller font
            int text_x1 = x_start + (button_width / 2) - (strlen(button_texts1[i]) * 7 / 2);
            int text_x2 = x_start + (button_width / 2) - (strlen(button_texts2[i]) * 7 / 2);
            TM_ILI9341_DrawFilledRectangle(x_start, y_pos, x_end, y_pos + button_height, BGcolor);
            TM_ILI9341_DrawRectangle(x_start, y_pos, x_end, y_pos + button_height, ILI9341_COLOR_WHITE);
            TM_ILI9341_Puts(text_x1, y_pos + (button_height / 2) - 10, button_texts1[i], &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_SLATE_BLUE);
            TM_ILI9341_Puts(text_x2, y_pos + (button_height / 2), button_texts2[i], &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_SLATE_BLUE);
        }
    }
}

void drawDMMUI() {
    // Clear the screen with a new dark blue background color
    TM_ILI9341_Fill(ILI9341_COLOR_LIGHT_GREEN_CYAN);

    // DMM title
    TM_ILI9341_Puts(5, 5, "Digital Multimeter (DMM)", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_LIGHT_GREEN_CYAN);

    // Main display area
    TM_ILI9341_DrawFilledRectangle(60, 20, 206, 125, ILI9341_COLOR_BLACK);
    TM_ILI9341_DrawRectangle(60, 20, 206, 125, ILI9341_COLOR_WHITE);
    TM_ILI9341_Puts(75, 60, "0.000", &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
    TM_ILI9341_Puts(180, 65, "V", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

    // "MENU" button
    TM_ILI9341_DrawFilledRectangle(5, 20, 55, 125, ILI9341_COLOR_MOSS_GREEN);
    TM_ILI9341_DrawRectangle(5, 20, 55, 125, ILI9341_COLOR_WHITE);
    TM_ILI9341_Puts(25, 32, "M", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
    TM_ILI9341_Puts(25, 54, "E", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
    TM_ILI9341_Puts(25, 76, "N", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);
    TM_ILI9341_Puts(25, 98, "U", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_MOSS_GREEN);

    drawDmmModesButtons(-1);

    int x_offset = 5;
    int button_width = 98;

    // Up and Down buttons for range selection, drawn with lines
    int btn_x = 258;
    int up_btn_mid_y = 50;
    int down_btn_mid_y = 145;
    x_offset = x_offset + 2 * (button_width + 5);

    // Up arrow
    TM_ILI9341_DrawFilledRectangle(x_offset, up_btn_mid_y-30, x_offset + button_width, up_btn_mid_y+48, ILI9341_COLOR_SLATE_BLUE);
    TM_ILI9341_DrawRectangle(x_offset, up_btn_mid_y-30, x_offset +  button_width, up_btn_mid_y+48, ILI9341_COLOR_WHITE);
    for (int y = 0; y < 15; y++) {
        TM_ILI9341_DrawLine(btn_x - y, up_btn_mid_y + y, btn_x + y, up_btn_mid_y + y, ILI9341_COLOR_WHITE);
    }

    // Down arrow
    TM_ILI9341_DrawFilledRectangle(x_offset, down_btn_mid_y-42, x_offset + button_width, down_btn_mid_y+35, ILI9341_COLOR_SLATE_BLUE);
    TM_ILI9341_DrawRectangle(x_offset, down_btn_mid_y-42, x_offset + button_width, down_btn_mid_y+35, ILI9341_COLOR_WHITE);
    down_btn_mid_y +=5;
    for (int y = 0; y < 15; y++) {
        TM_ILI9341_DrawLine(btn_x - y, down_btn_mid_y - y, btn_x + y, down_btn_mid_y - y, ILI9341_COLOR_WHITE);
    }
}

void drawDmmModesButtons(int mode) {
    // Mode selection buttons (Temperature, Capacitance, Voltage, Resistance, Current)
    int y_start = 185;
    int y_end = 235;
    int up_y_start = 130;
    int up_y_end = 180;
    int button_width = 98;
    int button_height = y_end - y_start;
    int x_offset = 5;

    // Correctly map dmmMode values to array indices
    uint16_t Bg_colors[5] = {
        ILI9341_COLOR_DARK_BLUE,  // Index 0: Temperature (dmmMode 1)
        ILI9341_COLOR_DARK_BLUE,  // Index 1: Capacitance (dmmMode 3)
        ILI9341_COLOR_DARK_BLUE,  // Index 2: Voltage (dmmMode 2)
        ILI9341_COLOR_DARK_BLUE,  // Index 3: Resistance (dmmMode 4)
        ILI9341_COLOR_DARK_BLUE   // Index 4: Current (dmmMode 5)
    };

    // Change the color of the selected mode button, since the ui and mode array isnot matchILI9341_COLOR_GREEN
    if (mode == 1) { // Temperature
        Bg_colors[0] = ILI9341_COLOR_MAGENTA;
    } else if (mode == 3) { // Capacitance
        Bg_colors[1] = ILI9341_COLOR_MAGENTA;
    } else if (mode == 2) { // Voltage
        Bg_colors[2] = ILI9341_COLOR_MAGENTA;
    } else if (mode == 4) { // Resistance
        Bg_colors[3] = ILI9341_COLOR_MAGENTA;
    } else if (mode == 5) { // Current
        Bg_colors[4] = ILI9341_COLOR_MAGENTA;
    }

    // Draw the top two buttons (Temperature and Capacitance)
    for (int i = 0; i < 2; i++) {
        TM_ILI9341_DrawFilledRectangle(x_offset + i * (button_width + 5), up_y_start, x_offset + i * (button_width + 5) + button_width, up_y_end, Bg_colors[i]);
        TM_ILI9341_DrawRectangle(x_offset + i * (button_width + 5), up_y_start, x_offset + i * (button_width + 5) + button_width, up_y_end, ILI9341_COLOR_WHITE);

        if (i == 0) { // Temperature button
            drawDegreeCSymbol(x_offset + (button_width / 2), up_y_start + (button_height / 2) - 12, 7, ILI9341_COLOR_WHITE, Bg_colors[i]);
            TM_ILI9341_Puts(x_offset + (button_width / 2) - 35, up_y_start + (button_height / 2) + 9, "Temperature", &TM_Font_7x10, ILI9341_COLOR_WHITE, Bg_colors[i]);
        } else { // Capacitance button
            TM_ILI9341_Puts(x_offset + i * (button_width + 5) + (button_width / 2) - 5, up_y_start + (button_height / 2) - 12, "F", &TM_Font_11x18, ILI9341_COLOR_WHITE, Bg_colors[i]);
            TM_ILI9341_Puts(x_offset + i * (button_width + 5) + (button_width / 2) - 35, up_y_start + (button_height / 2) + 9, "Capacitance", &TM_Font_7x10, ILI9341_COLOR_WHITE, Bg_colors[i]);
        }
    }

    // Draw the bottom three buttons (Voltage, Resistance, Current)
    for (int i = 0; i < 3; i++) {
        TM_ILI9341_DrawFilledRectangle(x_offset + i * (button_width + 5), y_start, x_offset + i * (button_width + 5) + button_width, y_end, Bg_colors[i + 2]);
        TM_ILI9341_DrawRectangle(x_offset + i * (button_width + 5), y_start, x_offset + i * (button_width + 5) + button_width, y_end, ILI9341_COLOR_WHITE);

        if (i == 1) { // Resistance button
            drawOmegaSymbol(x_offset + i * (button_width + 5) + (button_width / 2), y_start + (button_height / 2) - 9, 7, ILI9341_COLOR_WHITE, Bg_colors[i + 2]);
            TM_ILI9341_Puts(x_offset + i * (button_width + 5) + (button_width / 2) - 30, y_start + (button_height / 2) + 9, "Resistance", &TM_Font_7x10, ILI9341_COLOR_WHITE, Bg_colors[i + 2]);
        } else {
            TM_ILI9341_Puts(x_offset + i * (button_width + 5) + (button_width / 2) - 5, y_start + (button_height / 2) - 12, (i == 0) ? "V" : "A", &TM_Font_11x18, ILI9341_COLOR_WHITE, Bg_colors[i + 2]);
            TM_ILI9341_Puts(x_offset + i * (button_width + 5) + (button_width / 2) - 22, y_start + (button_height / 2) + 9, (i == 0) ? "Voltage" : "Current", &TM_Font_7x10, ILI9341_COLOR_WHITE, Bg_colors[i + 2]);
        }
    }
}

void drawOmegaSymbol(int x_pos, int y_pos, int radius, uint16_t color, uint16_t bg_color) {

	    // Draw proper Omega symbol- almost full circle with small opening
	    int center_x = x_pos ;
	    int center_y = y_pos + radius/2;

	    // Draw the full circle first
	    TM_ILI9341_DrawCircle(center_x, center_y, radius, color);
	    TM_ILI9341_DrawCircle(center_x, center_y, radius-1, color);

	    // Calculate dynamic dimensions based on radius
	    int blocking_width = radius / 2;
	    int blocking_height = radius / 2;
	    int foot_length = radius;

	    // Block the small part of the circle to create the opening
	    TM_ILI9341_DrawFilledRectangle(center_x - blocking_width, center_y + radius - 1, center_x + blocking_width, center_y + radius + blocking_height, bg_color);

	    // foots
	    TM_ILI9341_DrawFilledRectangle(center_x - blocking_width - foot_length, center_y + radius +1, center_x - blocking_width, center_y + radius, color);
	    TM_ILI9341_DrawFilledRectangle(center_x + blocking_width, center_y + radius +1, center_x + blocking_width+ foot_length, center_y + radius, color);
	}


void drawDegreeCSymbol(int x, int y, int size, uint16_t color, uint16_t background_color) {
	TM_FontDef_t* font;
    int circle_radius;
    // Select font and circle size based on the 'size' parameter
    switch (size) {
        case 1:
            font = &TM_Font_7x10;
            circle_radius = 2;
            break;
        case 2:
            font = &TM_Font_11x18;
            circle_radius = 3;
            break;
        case 3:
            // This option uses a slightly larger circle with the largest font
            font = &TM_Font_11x18;
            circle_radius = 4;
            break;
        default:
            // Default to a medium size if an invalid value is provided
            font = &TM_Font_11x18;
            circle_radius = 3;
            break;
    }

    // 1. Draw the degree symbol (small filled circle)
    TM_ILI9341_DrawCircle(x-4 , y, circle_radius, color);

    // 2. Draw the 'C' character next to the circle
    TM_ILI9341_Puts(x + circle_radius , y, "C", font, color, background_color);
}

//function to do gradient
uint16_t interpolate_color(uint16_t color1, uint16_t color2, float ratio) {
    // Extract RGB components
    uint8_t r1 = (color1 >> 11) & 0x1F;
    uint8_t g1 = (color1 >> 5) & 0x3F;
    uint8_t b1 = color1 & 0x1F;

    uint8_t r2 = (color2 >> 11) & 0x1F;
    uint8_t g2 = (color2 >> 5) & 0x3F;
    uint8_t b2 = color2 & 0x1F;

    // Interpolate each component
    uint8_t r = (uint8_t)(r1 + (r2 - r1) * ratio);
    uint8_t g = (uint8_t)(g1 + (g2 - g1) * ratio);
    uint8_t b = (uint8_t)(b1 + (b2 - b1) * ratio);

    // Recombine into 16-bit color
    return (r << 11) | (g << 5) | b;
}
