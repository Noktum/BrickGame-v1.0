#ifndef TETRIS_FRONTEND
#define TETRIS_FRONTEND

#include <ncurses.h>
#include <unistd.h>

#include "../../brick_game/brickgame.h"

void init_colors();
void print_color(WINDOW *win, int type, int i, int j);
void update_stats(WINDOW *stats);
void screen_init(WINDOW *keys, WINDOW *play_field, WINDOW *stats,
                 GameState_t state);
void redraw_field(WINDOW *field, WINDOW *stats, GameState_t state);
void gameloop(WINDOW *play_field, WINDOW *stats, GameState_t *state);
void gameover(WINDOW *play_field, WINDOW *stats);
void game_init();

#endif