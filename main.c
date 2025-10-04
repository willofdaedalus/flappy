#include "main.h"
#include "i2c.h"
#include "lcd.h"
#include <avr/interrupt.h>
#include <avr/io.h>
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
	DDRE &= ~_BV(PE2);
	DDRE |= _BV(PE6);
	PORTE |= _BV(PE2);
}

int main(void)
{
	// uint8_t old_row;

	_delay_ms(2000);

	gpio_init();
	timers_init();

	i2c_init();
	lcd_init();

	// lcd_set_cursor(player_row, 1);
	// lcd_display("@");


	sei();

	while (1) {
		lcd_send_cmd(LCD_CMD_CLEAR);
		// old_row = player_row;
		handle_input(&player_row);

		advance_frame(framebuf);
		draw_frame(framebuf);
		draw_player(player_row);

		_delay_ms(1000);
		// // draw player last
		// if (old_row != player_row) {
		// 	draw_player(player_row);
		// }
	}
}
