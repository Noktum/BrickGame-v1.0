#include "brick_game/brickgame.h"

#include "gui/cli/frontend.h"

int main() {
  srand(time(NULL));
  initscr();
  noecho();
  curs_set(0);  // invisible cursor
  raw();        // catch all keys
  game_init();
  endwin();
}
