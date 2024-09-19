#ifndef TETRIS_FRONTEND
#define TETRIS_FRONTEND

#include <ncurses.h>
#include <unistd.h>

#include "../../brick_game/brickgame.h"

void game_init();
void screen_init(WINDOW *keys, WINDOW *play_field, WINDOW *stats,
                 GameState_t state);
void redraw_field(WINDOW *field, WINDOW *stats, GameState_t state);
void gameloop(WINDOW *play_field, WINDOW *stats, int *c, GameState_t *state);
void update_stats(WINDOW *stats);
void init_colors();
void print_color(WINDOW *win, int type, int i, int j);
void gameover(WINDOW *play_field, WINDOW *stats);

#endif