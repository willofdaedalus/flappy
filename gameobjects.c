#include "lcd.h"
#include "main.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

#define MAX_OBJ_COUNT (2)

volatile uint8_t btn_state;
volatile uint8_t btn_event;
static uint8_t state = FB_BLANK;
static uint8_t run_len = 1;
// static uint8_t prev_obj;
// track the number of objs per obj that has been spawned
// static uint8_t tops, bottoms, blanks;

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

uint8_t btn_down(void)
{
	return btn_event;
}

// PLAYER STUFF
void handle_input(uint8_t *row)
{
	if (btn_down()) {
		*row = !(*row);
		btn_event = 0;
	}
}

void draw_player(uint8_t row)
{
	lcd_set_cursor(row, 1);
	lcd_display("@");
}

void advance_frame(uint8_t *framebuf)
{
	uint8_t i, c;

	for (i = 0; i < LCD_COLS - 1; i++) {
		c = framebuf[i + 1];
		framebuf[i] = c;
	}

	framebuf[i] = gen_obj();
}

void update_score(uint8_t *framebuf, uint8_t *score)
{
	static uint8_t counted = 0;

	if (framebuf[0] != FB_BLANK && counted == 0) {
		(*score)++;
		counted = 1;
	} else if (framebuf[0] == FB_BLANK) {
		counted = 0;
	}
}

uint8_t check_collision(uint8_t *framebuf, uint8_t player_row)
{
	return (player_row == 0 && framebuf[1] == FB_TOP_BLK) ||
	       (player_row == 1 && framebuf[1] == FB_BOT_BLK);
}

void draw_frame(uint8_t *framebuf)
{
	uint8_t x;

	for (x = 0; x < LCD_COLS; x++) {
		switch (framebuf[x]) {
		case FB_TOP_BLK:
			lcd_set_cursor(0, x);
			lcd_send_data(FB_PIPE);
			break;

		case FB_NO_BLK:
			lcd_set_cursor(0, x);
			lcd_send_data(FB_BLANK);
			break;

		case FB_BOT_BLK:
			lcd_set_cursor(1, x);
			lcd_send_data(FB_PIPE);
			break;
		}
	}
}

uint8_t gen_obj(void)
{
	uint8_t cur_obj = state;

	switch (state) {
	case FB_BLANK:
		if (run_len < 3) {
			cur_obj = FB_BLANK;
			run_len++;
		} else {
			// Must spawn a block now
			if (rand() % 2 == 0) {
				cur_obj = FB_TOP_BLK;
				state = FB_TOP_BLK;
			} else {
				cur_obj = FB_BOT_BLK;
				state = FB_BOT_BLK;
			}
			run_len = 1;
		}
		break;

	case FB_TOP_BLK:
		if (run_len < 2) {
			if (rand() % 2 == 0) {
				cur_obj = FB_TOP_BLK;
				run_len++;
			} else {
				cur_obj = FB_BLANK;
				state = FB_BLANK;
				run_len = 1;
			}
		} else {
			// must go to blank (can't go directly to bottom)
			cur_obj = FB_BLANK;
			state = FB_BLANK;
			run_len = 1;
		}
		break;

	case FB_BOT_BLK:
		if (run_len < 2) {
			if (rand() % 2 == 0) {
				cur_obj = FB_BOT_BLK;
				run_len++;
			} else {
				cur_obj = FB_BLANK;
				state = FB_BLANK;
				run_len = 1;
			}
		} else {
			cur_obj = FB_BLANK;
			state = FB_BLANK;
			run_len = 1;
		}
		break;
	}

	return cur_obj;
}
