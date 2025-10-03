#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define FB_NO_BLK (0)
#define FB_TOP_BLK (1)
#define FB_BOT_BLK (2)

#define LCD_COLS (16)

void handle_input(uint8_t *row);
void draw_player(uint8_t row);

#endif // _MAIN_H
