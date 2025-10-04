#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define FB_NO_BLK (0)
#define FB_TOP_BLK (1)
#define FB_BOT_BLK (2)

#define FB_PLAYER ("@")
#define FB_PIPE ("#")
#define FB_BOMB ("*")
#define FB_INV ("&")

#define LCD_COLS (16)

void handle_input(uint8_t *row);
void draw_player(uint8_t row);
void draw_frame(uint8_t *framebuf);

#endif // _MAIN_H
