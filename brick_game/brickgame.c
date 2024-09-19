#include "brickgame.h"

// reading high score when game starts
int read_high_score() {
  int score = 0;
  FILE *f = fopen("high_score.txt", "r");
  if (f) {
    fscanf(f, "%d", &score);
    fclose(f);
  }
  return score;
}

// if curr score is > than high => update file
void update_high_score_file(int score) {
  FILE *f = fopen("high_score.txt", "w");
  if (f) {
    fprintf(f, "%d", score);
    fclose(f);
  }
}

// init and edit game info
GameInfo_t *struct_init() {
  static GameInfo_t info;
  if (info.field == NULL) {
    srand(time(NULL));
    info.level = 1;
    info.score = 0;
    info.speed = 1000;
    info.high_score = read_high_score();
    info.field = malloc(22 * sizeof(int *));
    for (int i = 0; i < 22; i++) {
      info.field[i] = calloc(12, sizeof(int));
    }
    info.next = figure_alloc();
    Figure_t *figure = figure_init();
    copy_figure(figure->next, info.next);
    info.pause = 1;
  }
  return &info;
}

// return game info for frontend
GameInfo_t updateCurrentState() {
  GameInfo_t *info = struct_init();
  return *info;
}

// free field and next figure
void game_end() {
  GameInfo_t *info = struct_init();
  Figure_t *figure = figure_init();
  for (int i = 0; i < 22; i++) {
    free(info->field[i]);
  }
  free(info->field);

  for (int i = 0; i < 4; i++) {
    free(info->next[i]);
    free(figure->next[i]);
    free(figure->figure[i]);
  }
  free(figure->next);
  free(figure->figure);
  free(info->next);
}

// check if figure collides with others under it
bool check_collide() {
  Figure_t *figure = figure_init();
  GameInfo_t *info = struct_init();
  int y = figure->y;
  int x = figure->x;
  bool collide = false;
  for (int i = 0; i < 4 && !collide; i++) {
    for (int j = 0; j < 4 && !collide; j++) {
      if (figure->figure[i][j] != 0 &&
          (info->field[y - 2 + i][x - 1 + j] != 0 || y - 2 + i > 20)) {
        collide = true;
      }
    }
  }
  return collide;
}

// delete string from field
void delete_string(int str) {
  GameInfo_t *info = struct_init();
  for (int i = 20; i >= 0; i--) {
    for (int j = 1; j <= 10; j++) {
      if (i <= str && i > 0)
        info->field[i][j] = info->field[i - 1][j];
      else if (i == 0)
        info->field[i][j] = 0;
    }
  }
}

// counting score, level and speed of game
void score_fnc() {
  GameInfo_t *info = struct_init();
  int number = 0;
  int flag = 0;
  for (int i = 1; i <= 20; i++) {
    for (int j = 1; j <= 10; j++) {
      if (info->field[i][j] == 0) {
        flag = 1;
        break;
      }
    }
    if (flag == 0) {
      delete_string(i);
      number++;
    }
    flag = 0;
  }
  if (number == 1)
    info->score += 100;
  else if (number == 2)
    info->score += 300;
  else if (number == 3)
    info->score += 700;
  else if (number == 4)
    info->score += 1500;
  if (info->score > info->high_score) {
    info->high_score = info->score;
    update_high_score_file(info->high_score);
  }
  if (info->level * 600 <= info->score) {
    info->level = info->score / 600 + 1;
    info->speed -= 100;
  }
}

void finite_state_machine(UserAction_t action, GameState_t *state, int *flag) {
  Figure_t *figure = figure_init();
  GameInfo_t *info = struct_init();
  if (*state == MOVING) {
    if (action == Pause) {
      *state = PAUSE;
    }
    figure->y++;
    bool collide = check_collide(info->field);
    if (collide) {
      figure->y--;
      *state = ATTACH;
    }
  } else if (*state == ATTACH) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (figure->figure[i][j]) {
          info->field[figure->y - 2 + i][figure->x - 1 + j] =
              figure->figure[i][j];
        }
      }
    }
    score_fnc();
    *state = SPAWN;
  } else if (*state == SPAWN) {
    figure_respawn(figure);
    copy_figure(figure->next, info->next);
    bool collide = check_collide(info->field);
    if (collide) {
      *state = GAMEOVER;
    } else {
      *state = MOVING;
    }
  } else if (*state == START && action == Start) {
    *state = SPAWN;
  } else if (*state == GAMEOVER) {
    *flag = false;
  } else if (*state == PAUSE) {
    if (action == Pause) {
      *state = MOVING;
    }
  }
}

// idk what to do with that
void userInput(UserAction_t action, bool hold) {
  Figure_t *figure = figure_init();
  switch (action) {
    case Start:
      break;
    case Pause:
      break;
    case Terminate:
      break;
    case Left:
      move_left();
      break;
    case Right:
      move_right();
      break;
    case Up:
      break;
    case Down:
      while (!check_collide() && hold) {
        move_down();
      }
      move_up();
      break;
    case Action:
      rotate_right(figure->figure);
      break;
    default:
      break;
  }
}
