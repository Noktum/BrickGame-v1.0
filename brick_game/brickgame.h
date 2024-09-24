#ifndef BRICKGAME
#define BRICKGAME

/// @file brickgame.h

#include <stdbool.h>

#include "tetris/backend.h"

/// @brief Enumeration of all possible user actions
typedef enum {
  Start,      ///< Launches the game from START state
  Pause,      ///< Pause and resume action
  Terminate,  ///< Exit from game with GAMEOVER state
  Left,       ///< Moves the figure left
  Right,      ///< Moves the figure right
  Up,         ///< Doing nothing in this version
  Down,       ///< Moves the figure straight
  Action      ///< Rotates the figure clockwise
} UserAction_t;

/// @brief Structure, that contains game stats and information
typedef struct {
  int **field;     ///< Game field with fallen figures
  int **next;      ///< Next figure, displayed in states
  int score;       ///< Playes score in current game
  int high_score;  ///< Highest score through all played games
  int level;       ///< Current level
  int speed;       ///< Game speed
  int pause;  ///< Pause info, possible values: 0 - play, 1 - pause, 2 - exit
} GameInfo_t;

/// @brief Enumeration of possible game states
typedef enum {
  START,     ///< waits till enter is pressed
  PAUSE,     ///< pause
  SPAWN,     ///< spawn new figure on field
  MOVING,    ///< moving figure down
  SHIFTING,  ///< moment where fig is movin left/right
  GAMEOVER,  ///< gameover through overflow
  WIN,       ///< level 11 is reached
  ATTACH     ///< copy figure to field
} GameState_t;

int read_high_score();
void update_high_score_file(int score);
GameInfo_t *struct_init();
GameInfo_t updateCurrentState();
void game_end();
bool check_collide();
void delete_string(int str);
void score_fnc();
GameState_t *state_getter();
void shifting(UserAction_t action, bool hold);
void moving(clock_t *start, clock_t *end);
void userInput(UserAction_t action, bool hold);

#endif