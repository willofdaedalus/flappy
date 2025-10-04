#include "lcd.h"
#include "main.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t btn_state;
volatile uint8_t btn_event;

ISR(TIMER0_COMPA_vect)
{
	static uint8_t counter = 0;
	static uint8_t last_raw = 1;

	uint8_t raw = bit_is_set(PINE, PE2);

	if (raw == last_raw) {
		if (counter < 5)
			counter++;
		else if (raw != btn_state) {
			btn_state = raw;
			if (btn_state == 0)
				btn_event = 1;
		}
	} else {
		counter = 0;
	}

	last_raw = raw;
}

// PLAYER STUFF
void handle_input(uint8_t *row)
{
	if (btn_event) {
		*row = !(*row);
		btn_event = 0;
	}
}

void draw_player(uint8_t row)
{
	// clear both rows first
	// without this we leave a "ghost" of the previous render
	lcd_set_cursor(0, 1);
	lcd_display(" ");
	lcd_set_cursor(1, 1);
	lcd_display(" ");

	// draw player
	lcd_set_cursor(row, 1);
	lcd_display("@");
}

// PIPE STUFF
// void draw_pipe(uint8_t pipe, uint8_t col)
// {
// 	switch (pipe) {
// 	case FB_TOP_BLK:
// 		break;
//
// 	case FB_NO_BLK:
// 		break;
//
// 	case FB_BOT_BLK:
// 		break;
// 	}
// }

static inline void clear_cell(uint8_t x)
{
	lcd_set_cursor(x, 0);
	lcd_display(" ");
	lcd_set_cursor(x, 1);
	lcd_display(" ");
}

void draw_frame(uint8_t *framebuf)
{
	uint8_t x;

	for (x = 0; x < LCD_COLS; x++) {
		switch (framebuf[x]) {
		case FB_TOP_BLK:
			// clear_cell(x);
			lcd_set_cursor(0, x);
			lcd_display(FB_PIPE);
			break;

		case FB_NO_BLK:
			// clear_cell(x);
			lcd_set_cursor(0, x);
			lcd_display(" ");
			break;

		case FB_BOT_BLK:
			// clear_cell(x);
			lcd_set_cursor(1, x);
			lcd_display(FB_PIPE);
			break;
		}
	}
}
