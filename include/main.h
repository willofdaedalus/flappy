#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define FB_NO_BLK (0)
#define FB_TOP_BLK (1)
#define FB_BOT_BLK (2)

#define FB_PLAYER (0x40) // @
#define FB_PIPE (0x23)	 // #
#define FB_BOMB (0x2a)	 // *
#define FB_INV (0x26)	 // &
#define FB_BLANK (0x20)	 // SPACE

#define LCD_COLS (16)

enum GameState {
	GS_TitleScreen,
	GS_MainLoop,
	GS_ScoreScreen,
};

void handle_input(uint8_t *row);
void draw_player(uint8_t row);
void draw_frame(uint8_t *framebuf);
uint8_t gen_obj(void);
void advance_frame(uint8_t *framebuf);
uint8_t check_collision(uint8_t *framebuf, uint8_t player_row);

#endif // _MAIN_H
