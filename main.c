#include "main.h"
#include "i2c.h"
#include "lcd.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>


// each value represents how the column is rendered
// 0 - no blocks on that column
// 1 - block on the top
// 2 - block on the bottom
// 3 - destroy blocks powerup
// 4 - invincibility powerup
static uint8_t framebuf[LCD_COLS];

static uint8_t player_row;

static void timers_init(void)
{
	// 1024 prescaler
	TCCR0B = _BV(CS00) | _BV(CS01);
	// ctc mode
	TCCR0A = _BV(WGM01);
	// ~16ms
	OCR0A = 249;
	// comp match a interrupt
	TIMSK0 = _BV(OCIE0A);
}

void gpio_init(void)
{
	// initialize button
	DDRE &= ~_BV(PE2);
	PORTE |= _BV(PE2);
}

int main(void)
{
	// uint8_t old_row;

	char buf[8];
	uint8_t is_highscore = 0;
	uint8_t score = 0, highscore = 0, i;
	enum GameState state = GS_TitleScreen;
	_delay_ms(2000);

	gpio_init();
	timers_init();

	i2c_init();
	lcd_init();


	// lcd_set_cursor(player_row, 1);
	// lcd_display("@");


	sei();

	while (1) {
		switch (state) {
		case GS_TitleScreen:
			lcd_set_cursor(0, 1);
			lcd_display("dodger game");

			lcd_set_cursor(1, 1);
			lcd_display("@willofdaedalus");
			_delay_ms(5000);
			state = GS_MainLoop;
			break;

		case GS_MainLoop:
			lcd_send_cmd(LCD_CMD_CLEAR);
			lcd_send_cmd(LCD_CURSOR_OFF);
			handle_input(&player_row);

			advance_frame(framebuf);
			draw_frame(framebuf);
			draw_player(player_row);

			if (check_collision(framebuf, player_row)) {
				lcd_send_cmd(LCD_CMD_CLEAR);
				lcd_send_cmd(LCD_CMD_HOME);
				state = GS_ScoreScreen;
			}

			update_score(framebuf, &score);

			_delay_ms(50);
			break;

		case GS_ScoreScreen:
			lcd_send_cmd(LCD_CMD_CLEAR);

			if (score > highscore || is_highscore) {
				lcd_display("new highscore");
				highscore = score;
				is_highscore = 1;
				snprintf(buf, sizeof(buf), "%u", highscore);
			} else {
				lcd_display("your score");
				snprintf(buf, sizeof(buf), "%u", score);
			}

			lcd_set_cursor(1, 1);
			lcd_display(buf);
			_delay_ms(1000);

			if (btn_down()) {
				lcd_init();
				score = 0;
				player_row = 0;
				for (i = 0; i < LCD_COLS; i++)
					framebuf[i] = 0;

				state = GS_MainLoop;
				is_highscore = 0;
			}
			break;
		}
	}
}
