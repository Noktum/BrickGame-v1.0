#ifndef BRICKGAME
#define BRICKGAME

#include <stdbool.h>

#include "tetris/backend.h"

/* delete this block? */
#define FIELD_X 12
#define FIELD_Y 22
#define STEP 1
#define PAUSE_KEY 'p'
#define ROTATE_KEY_R 'e'
#define ROTATE_KEY_L 'q'

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,  // don't needed there
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;  // 0 - play, 1 - pause, 2 - exit
} GameInfo_t;

typedef enum {
  START,     // waits till enter is pressed
  PAUSE,     // pause
  SPAWN,     // spawn new figure on field
  MOVING,    // moving figure down
  SHIFTING,  // moment where fig is movin left/right
  GAMEOVER,  // gameover through overflow
  WIN,       // level 11 is reached
  ATTACH,    // copy figure to field
  EXIT       // exit by win, gameover or key
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