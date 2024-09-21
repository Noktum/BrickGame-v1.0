#include "frontend.h"

// creates pairs of colors for figures
void init_colors() {
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_CYAN);
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(3, COLOR_RED, COLOR_RED);
  init_pair(4, COLOR_GREEN, COLOR_GREEN);
  init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(6, COLOR_WHITE, COLOR_WHITE);
  init_pair(7, COLOR_BLUE, COLOR_BLUE);
}

// prints colored "pixels"
void print_color(WINDOW *win, int type, int i, int j) {
  wattron(win, COLOR_PAIR(type));
  mvwprintw(win, i, j, "#");
  wattroff(win, COLOR_PAIR(type));
}

// updates windows with game statistics
void update_stats(WINDOW *stats) {
  GameInfo_t info = updateCurrentState();
  mvwprintw(stats, 0, 6, "STATISTICS");
  mvwprintw(stats, 2, 2, "SCORE: %d", info.score);
  mvwprintw(stats, 3, 2, "HIGH SCORE: %d", info.high_score);
  mvwprintw(stats, 5, 2, "LEVEL: %d", info.level);
  mvwprintw(stats, 6, 2, "NEXT FIGURE: ");
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_CYAN);
  for (int i = 0; i < 4 && info.next != NULL; i++) {
    for (int j = 0; j < 4; j++) {
      if (info.next[i][j]) {
        print_color(stats, info.next[i][j], 7 + i, 3 + j * 2 + 1);
        print_color(stats, info.next[i][j], 7 + i, 3 + (j + 1) * 2);
      } else {
        mvwprintw(stats, 7 + i, 3 + j * 2 + 1, " ");
        mvwprintw(stats, 7 + i, 3 + (j + 1) * 2, " ");
      }
    }
  }
  wrefresh(stats);
}

// initialize all windows: stats, keys and play field
void screen_init(WINDOW *keys, WINDOW *play_field, WINDOW *stats,
                 GameState_t state) {
  box(keys, 0, 0);
  box(play_field, 0, 0);
  box(stats, 0, 0);
  if (state != START) {
    update_stats(stats);
  }

  mvwprintw(keys, 0, 9, "KEYS");
  mvwprintw(keys, 1, 2, "ENTER - start game");
  mvwprintw(keys, 2, 2, "O - quit game");
  mvwprintw(keys, 3, 2, "P - pause");
  mvwprintw(keys, 5, 2, "< - move left");
  mvwprintw(keys, 6, 2, "> - move right");
  mvwprintw(keys, 7, 2, "v - move down");
  mvwprintw(keys, 8, 2, "E - rotate right");

  mvwprintw(play_field, 0, 8, "TETRIS");
  wrefresh(keys);
  wrefresh(play_field);
}

// redraws play field on each tick
void redraw_field(WINDOW *field, WINDOW *stats, GameState_t state) {
  Figure_t *figure = figure_init();
  GameInfo_t info = updateCurrentState();

  for (int i = 1; i <= 20 && info.field; i++) {
    for (int j = 1; j <= 10; j++) {
      if (info.field[i][j]) {
        print_color(field, info.field[i][j], i, j * 2 - 1);
        print_color(field, info.field[i][j], i, j * 2);
      } else {
        mvwaddch(field, i, j * 2 - 1, ' ');
        mvwaddch(field, i, j * 2, ' ');
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (figure->figure[i][j]) {
        print_color(field, figure->figure[i][j], figure->y - 2 + i,
                    (figure->x - 1 + j) * 2 - 1);
        print_color(field, figure->figure[i][j], figure->y - 2 + i,
                    (figure->x - 1 + j) * 2);
      }
    }
  }
  box(field, 0, 0);
  mvwprintw(field, 0, 8, "TETRIS");
  if (state != START) {
    update_stats(stats);
  }
  wrefresh(field);
}

// main loop with player input and drawing interface
void gameloop(WINDOW *play_field, WINDOW *stats, GameState_t *state) {
  GameInfo_t info = updateCurrentState();
  UserAction_t action = -1;
  bool hold = FALSE;
  clock_t start = clock() - CLOCKS_PER_SEC;
  while (info.pause != 2) {
    info = updateCurrentState();
    int c = wgetch(play_field);
    if (c == 'o' || c == 'O') {
      action = Terminate;
    } else if (c == '\n') {
      action = Start;
    } else if (c == 'p' || c == 'P') {
      action = Pause;
    } else if (c == KEY_DOWN) {
      action = Down;
      hold = true;
    } else if (c == KEY_LEFT) {
      action = Left;
    } else if (c == KEY_RIGHT) {
      action = Right;
    } else if (c == 'e' || c == 'E') {
      action = Action;
    }
    clock_t end;
    userInput(action, hold);
    moving(&start, &end);
    // if (action > 0 || end - start >= (long unsigned int)info.speed)
    redraw_field(play_field, stats, *state);
    action = -1;
    hold = FALSE;
  }
}

// will write player stats when game is finished
void gameover(WINDOW *play_field, WINDOW *stats) {
  mvwprintw(play_field, 10, 2, "GAME OVER");
  update_stats(stats);
  wrefresh(play_field);
  game_end();
  wgetch(play_field);
}

// creates windows and launches all other functions
void game_init() {
  WINDOW *keys = newwin(10, 22, 12, 0);
  WINDOW *play_field = newwin(22, 22, 0, 22);
  WINDOW *stats = newwin(12, 22, 0, 0);
  init_colors();
  GameState_t *state = state_getter();
  screen_init(keys, play_field, stats, *state);
  keypad(play_field, TRUE);
  nodelay(play_field, TRUE);
  gameloop(play_field, stats, state);
  gameover(play_field, stats);
  delwin(keys);
  delwin(play_field);
  delwin(stats);
}