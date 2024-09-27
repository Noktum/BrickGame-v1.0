#ifndef TETRIS
#define TETRIS

/// @file
/// @brief Declaration of Figure_t structure and functions used with it

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// @brief Structure for falling figures
typedef struct {
  int **figure;     ///< Current figure in the game
  int **next;       ///< Next generated figure in game stats windows
  int x;            ///< Horizontal coordinate of figure
  int y;            ///< Vertical coordinate of figure
  int next_type;    ///< Type of next figure for creating it
  int next_rotate;  ///< Random rotation of next figure
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