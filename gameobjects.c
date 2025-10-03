#include "lcd.h"
#include "main.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define BUTTON (PE2)

volatile uint8_t btn_state;
volatile uint8_t btn_event;

ISR(TIMER0_COMPA_vect)
{
	static uint8_t counter = 0;
	static uint8_t last_raw = 1;

	uint8_t raw = bit_is_set(PINE, BUTTON);

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
