#ifndef TETRIS
#define TETRIS

/// @file backend.h
/// @brief Declarates used functions and Figure_t structure

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// @brief structude for falling figures
typedef struct {
  int **figure;     ///< current figure in the game
  int **next;       ///< next generated figure in game stats windows
  int x;            ///< horizontal coordinate of figure
  int y;            ///< vertical coordinate of figure
  int type;         ///< type of figure for frontend
  int next_type;    ///< type of next figure for creating it
  int next_rotate;  ///< random rotation of next figure
} Figure_t;

Figure_t *figure_init();
int **figure_alloc();
void copy_figure(int **orig, int **copy);
void clear_figure(int **matrix);
char check_border();
void move_left();
void move_right();
void move_down();
void move_up();
void rotate_right(int **matrix);
void rotate_left(int **matrix);
void spawn();
void figure_respawn();

#endif