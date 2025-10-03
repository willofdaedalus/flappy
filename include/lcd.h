#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// PCF8574 I2C LCD pin mappings
#define LCD_RS_MASK (0x01)   // Register select
#define LCD_RW_MASK (0x02)   // Read/Write
#define LCD_EN_MASK (0x04)   // Enable
#define LCD_BL_MASK (0x08)   // Backlight
#define LCD_DATA_MASK (0xf0) // Data bits (D4-D7)

// Register select modes
#define LCD_RS_CMD (0x00)	// Command mode
#define LCD_RS_DATA LCD_RS_MASK // Data mode

// LCD Commands
#define LCD_CMD_CLEAR (0x01)
#define LCD_CMD_HOME (0x02)
#define LCD_CMD_ENTRY_MODE (0x04)
#define LCD_CMD_DISPLAY_CTRL (0x08)
#define LCD_CMD_SHIFT (0x10)
#define LCD_CMD_FUNCTION_SET (0x20)
#define LCD_CMD_CGRAM_ADDR (0x40)
#define LCD_CMD_DDRAM_ADDR (0x80)

// Entry mode flags
#define LCD_ENTRY_RIGHT (0x00)
#define LCD_ENTRY_LEFT (0x02)
#define LCD_ENTRY_SHIFT_INC (0x01)
#define LCD_ENTRY_SHIFT_DEC (0x00)

// Display control flags
#define LCD_DISPLAY_ON (0x04)
#define LCD_DISPLAY_OFF (0x00)
#define LCD_CURSOR_ON (0x02)
#define LCD_CURSOR_OFF (0x00)
#define LCD_BLINK_ON (0x01)
#define LCD_BLINK_OFF (0x00)

// Function set flags
#define LCD_4BIT_MODE (0x00)
#define LCD_8BIT_MODE (0x10)
#define LCD_1LINE (0x00)
#define LCD_2LINE (0x08)
#define LCD_5x8_FONT (0x00)
#define LCD_5x10_FONT (0x04)

// LCD dimensions
#define LCD_ROWS (2)
#define LCD_COLS (16)

// Function prototypes
void lcd_init(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_display(const char *str);
void lcd_clear(void);
void lcd_home(void);

#endif // LCD_H
