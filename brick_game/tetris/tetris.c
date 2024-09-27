#include "backend.h"

/// @file
/// @brief Implementation of functions from backend.h
/// @details

/// @brief Initializes falling figure and its information
/// @return Figure_t structure
Figure_t *figure_init() {
  static Figure_t figure;
  if (figure.figure == NULL) {
    figure.x = 5;
    figure.y = 2;
    figure.figure = figure_alloc();
    figure.next = figure_alloc();
    figure.next_type = rand() % 7;
    figure.next_rotate = rand() % 4;
    spawn();
  }
  return &figure;
}

/// @brief Allocates memory for matrix 4x4 (all figures in game)
/// @return Pointer to allocated memory
int **figure_alloc() {
  int **matrix = malloc(4 * sizeof(int *));
  for (int i = 0; i < 4; i++) {
    matrix[i] = calloc(4, sizeof(int));
  }
  return matrix;
}

/// @brief Copies figure to another matrix
/// @param orig - original matrix
/// @param copy - where it will be copied
void copy_figure(int **orig, int **copy) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      copy[i][j] = orig[i][j];
    }
  }
}

/// @brief Clears the matrix - sets 0 to all cells
/// @param matrix - matrix that will be cleaned
void clear_figure(int **matrix) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix[i][j] = 0;
    }
  }
}

/// @brief Checks is the figure collides into border or not
/// @return 0 if it doesn't collide, 1 - collides in left, 2 - in right ones
char check_border() {
  Figure_t *figure = figure_init();
  char collapse = 0;
  for (int i = 0; i < 4 && !collapse; i++) {
    for (int j = 0; j < 4 && !collapse; j++) {
      if (figure->figure[i][j] && figure->x - 1 + j < 1) {
        collapse = 1;
      } else if (figure->figure[i][j] && figure->x - 1 + j > 10) {
        collapse = 2;
      }
    }
  }
  return collapse;
}

/* MOVE TO DIFF DIRECTIONS */

/// @brief Moves figure 1 step left
void move_left() {
  Figure_t *figure = figure_init();
  figure->x--;
  if (check_border() == 1) {
    figure->x++;
  }
}

/// @brief Moves figure 1 step right
void move_right() {
  Figure_t *figure = figure_init();
  figure->x++;
  if (check_border() == 2) {
    figure->x--;
  }
}

/// @brief Moves figure 1 step down
void move_down() {
  Figure_t *figure = figure_init();
  figure->y++;
}

/// @brief Moves figure 1 step up
void move_up() {
  Figure_t *figure = figure_init();
  figure->y--;
}

/***************************/

/// @brief Rotates the figure clockwise
/// @param matrix - figure matrix
void rotate_right(int **matrix) {
  int **copy = figure_alloc();
  copy_figure(matrix, copy);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix[i][3 - j] = copy[j][i];
    }
  }
  for (int i = 0; i < 4; i++) {
    free(copy[i]);
  }
  free(copy);
}

/// @brief Rotates the figure counterclockwise
/// @param matrix - figure matrix
void rotate_left(int **matrix) {
  int **copy = figure_alloc();
  copy_figure(matrix, copy);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix[3 - i][j] = copy[j][i];
    }
  }
  for (int i = 0; i < 4; i++) {
    free(copy[i]);
  }
  free(copy);
}

/// @brief Fills right cells in matrix to create certain figure
void spawn() {
  Figure_t *figure = figure_init();
  int **matrix = figure->next;
  clear_figure(matrix);
  switch (figure->next_type) {
    case 0:  // I
      matrix[1][0] = matrix[1][1] = matrix[1][2] = matrix[1][3] = 1;
      break;
    case 1:  // J
      matrix[1][1] = matrix[2][1] = matrix[2][2] = matrix[2][3] = 2;
      break;
    case 2:  // L
      matrix[1][2] = matrix[2][2] = matrix[2][1] = matrix[2][0] = 3;
      break;
    case 3:  // O
      matrix[1][1] = matrix[1][2] = matrix[2][1] = matrix[2][2] = 4;
      break;
    case 4:  // S
      matrix[1][2] = matrix[1][1] = matrix[2][1] = matrix[2][0] = 5;
      break;
    case 5:  // T
      matrix[1][1] = matrix[2][0] = matrix[2][1] = matrix[2][2] = 6;
      break;
    case 6:  // Z
      matrix[1][1] = matrix[1][2] = matrix[2][2] = matrix[2][3] = 7;
      break;
  }
  for (int i = 0; i < figure->next_rotate && figure->next_type != 3; i++) {
    rotate_right(matrix);
  }
}

/// @brief Recreates figure on field, generates next figure
void figure_respawn() {
  Figure_t *figure = figure_init();
  figure->x = 5;
  figure->y = 2;

  copy_figure(figure->next, figure->figure);
  figure->next_type = rand() % 7;
  figure->next_rotate = rand() % 4;
  spawn();
}