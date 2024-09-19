#ifndef TETRIS
#define TETRIS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int **figure;
  int **next;
  int x;
  int y;
  int type;
  int next_type;
  int next_rotate;
} Figure_t;

Figure_t *figure_init();
int **figure_alloc();
void copy_figure(int **orig, int **copy);
void clear_figure(int **matrix);
bool check_border();
void move_left();
void move_right();
void move_down();
void move_up();
void rotate_right(int **matrix);
void spawn();
void figure_respawn();

#endif