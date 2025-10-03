#include "lcd.h"
#include "i2c.h"
#include <util/delay.h>

#define LCD_I2C_ADDR (0x27)

static uint8_t display_ctrl = LCD_CMD_DISPLAY_CTRL | LCD_DISPLAY_ON;

// low-level i2c write with start and stop
static void lcd_i2c_write(uint8_t data)
{
	i2c_start();
	i2c_sla_tx(LCD_I2C_ADDR, TW_WRITE);
	i2c_send_data(data);
	i2c_stop();
}

static void lcd_tx(uint8_t data, uint8_t type)
{
	uint8_t high = (data & LCD_DATA_MASK) | type;
	uint8_t low = ((data << 4) & LCD_DATA_MASK) | type;

	// send high nibble
	lcd_i2c_write(high | LCD_BL_MASK);
	lcd_i2c_write(high | LCD_BL_MASK | LCD_EN_MASK); // e high
	_delay_us(1);
	lcd_i2c_write(high | LCD_BL_MASK); // e low
	_delay_us(50);

	// send low nibble
	lcd_i2c_write(low | LCD_BL_MASK);
	lcd_i2c_write(low | LCD_BL_MASK | LCD_EN_MASK);
	_delay_us(1);
	lcd_i2c_write(low | LCD_BL_MASK);
	_delay_us(50);
}

static void lcd_tx_nib(uint8_t nibble, uint8_t control)
{
	uint8_t data;

	data = (nibble & 0xF0) | control | LCD_BL_MASK;

	// send with en low
	lcd_i2c_write(data);
	// pulse en high
	lcd_i2c_write(data | LCD_EN_MASK);
	_delay_us(1);

	// back to en low
	lcd_i2c_write(data & ~LCD_EN_MASK);
	_delay_us(50);
}

void lcd_send_cmd(uint8_t cmd)
{
	lcd_tx(cmd, LCD_RS_CMD);

	// Clear and home commands need longer delay
	if (cmd == LCD_CMD_CLEAR || cmd == LCD_CMD_HOME) {
		_delay_ms(2);
	} else {
		_delay_us(100);
	}
}

void lcd_send_data(uint8_t data)
{
	lcd_tx(data, LCD_RS_DATA);
	_delay_us(100);
}

void lcd_init(void)
{
	// wait for lcd to power up
	_delay_ms(50);

	lcd_tx_nib(0x30, LCD_RS_CMD);
	_delay_ms(5);

	lcd_tx_nib(0x30, LCD_RS_CMD);
	_delay_us(150);

	lcd_tx_nib(0x30, LCD_RS_CMD);
	_delay_us(150);

	// switch to 4-bit mode
	lcd_tx_nib(0x20, LCD_RS_CMD);
	_delay_us(150);

	// now use full byte commands
	// function set: 4-bit, 2-line, 5×8 font
	lcd_send_cmd(LCD_CMD_FUNCTION_SET | LCD_4BIT_MODE | LCD_2LINE |
		     LCD_5x8_FONT);

	// Display off
	lcd_send_cmd(LCD_CMD_DISPLAY_CTRL | LCD_DISPLAY_OFF);

	// clear display
	lcd_send_cmd(LCD_CMD_CLEAR);

	// entry mode set: increment, no shift
	// lcd_send_cmd(LCD_CMD_ENTRY_MODE | LCD_ENTRY_LEFT |
	// LCD_ENTRY_SHIFT_DEC);

	display_ctrl = LCD_CMD_DISPLAY_CTRL | LCD_DISPLAY_ON;

	lcd_send_cmd(display_ctrl);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
	if (row >= LCD_ROWS)
		row = LCD_ROWS - 1;
	if (col >= LCD_COLS)
		col = LCD_COLS - 1;

	// calculate ddram address
	// row 0 starts at 0x00, row 1 starts at 0x40
	uint8_t address = (row == 0) ? 0x00 : 0x40;
	address += col;

	// set ddram address
	lcd_send_cmd(LCD_CMD_DDRAM_ADDR | address);
}

void lcd_display(const char *str)
{
	while (*str) {
		lcd_send_data(*str++);
	}
}

void lcd_clear(void)
{
	lcd_send_cmd(LCD_CMD_CLEAR);
}
