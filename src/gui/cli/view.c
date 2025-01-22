#include "view.h"

#include "../../brick_game/defines.h"

int main() {
  int error = EXIT_SUCCESS;

  if (initscr()) {
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(50);
    setlocale(LC_ALL, "");
    print_overlay();
    game_loop();
    endwin();

  } else {
    error = EXIT_FAILURE;
  }

  return error;
}

void game_loop() {
  GameInfo_t game;
  int error = EXIT_SUCCESS;

  while (error != EXIT_FAILURE) {
    game = updateCurrentState();
    if ((error = print_field(&game))) {
      continue;
    }

    print_next(&game);
    print_pause(game.pause);
    print_stats(&game);
    UserAction_t userAct = (UserAction_t)getch();
    get_signal(userAct, &userAct);

    userInput(userAct, false);
  }
}

// получение сигналов от пользователя
void get_signal(int userIn, UserAction_t *action) {
  if (userIn == UP)
    *action = Up;
  else if (userIn == DOWN)
    *action = Down;
  else if (userIn == LEFT)
    *action = Left;
  else if (userIn == RIGHT)
    *action = Right;
  else if (userIn == EXIT_Q)
    *action = Terminate;
  else if (userIn == START_S)
    *action = Start;
  else if (userIn == SPACE)
    *action = Action;
  else if (userIn == PAUSE_P)
    *action = Pause;
}

// печать поля
void print_overlay() {
  print_rectangle(0, BOARD_N + 1, 1, (BOARD_M * 2) + 2);  // left table
  print_rectangle(0, BOARD_N + 1, (BOARD_M + 2) * 2,
                  (BOARD_M * 2) + HUD_WIDTH + 6);  // right
  print_rectangle(1, 3, (BOARD_M * 2) + 5, (BOARD_M * 2) + HUD_WIDTH + 5);
  print_rectangle(4, 6, (BOARD_M * 2) + 5, (BOARD_M * 2) + HUD_WIDTH + 5);
  print_rectangle(7, 10, (BOARD_M * 2) + 5, (BOARD_M * 2) + HUD_WIDTH + 5);
  print_rectangle(11, 13, (BOARD_M * 2) + 5, (BOARD_M * 2) + HUD_WIDTH + 5);
  print_rectangle(14, 17, (BOARD_M * 2) + 5, (BOARD_M * 2) + HUD_WIDTH + 5);

  MVPRINTW(2, (BOARD_M * 2) + 7, "LEVEL");
  MVPRINTW(5, (BOARD_M * 2) + 7, "SCORE");
  MVPRINTW(8, (BOARD_M * 2) + 7, "HIGH");
  MVPRINTW(9, (BOARD_M * 2) + 7, "SCORE");
  MVPRINTW(12, (BOARD_M * 2) + 7, "SPEED");
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_stats(GameInfo_t *game) {
  MVPRINTW(2, BOARD_M * 2 + 13, "    ");
  MVPRINTW(2, BOARD_M * 2 + 13, "%d", game->level);
  MVPRINTW(5, BOARD_M * 2 + 13, "    ");
  MVPRINTW(5, BOARD_M * 2 + 13, "%d", game->score);
  MVPRINTW(9, BOARD_M * 2 + 13, "    ");
  MVPRINTW(9, BOARD_M * 2 + 13, "%d", game->high_score);
  MVPRINTW(12, BOARD_M * 2 + 13, "    ");
  MVPRINTW(12, BOARD_M * 2 + 13, "%d", game->speed);
}

bool print_field(GameInfo_t *game) {
  bool error = EXIT_SUCCESS;
  if (game->field != NULL) {
    for (int i = 0; i < BOARD_N; i++) {
      for (int j = 0; j < BOARD_M; j++) {
        if (game->field[i][j] == 0)
          MVPRINTW(i + 1, j * 2 + 2, "  ");
        else if (game->field[i][j] == 1)
          MVPRINTW(i + 1, j * 2 + 2, "[]");
      }
    }
  } else
    error = EXIT_FAILURE;
  return error;
}

void print_next(GameInfo_t *game) {
  if (game->next != NULL) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 4; j++) {
        if (game->next[i][j] == 0)
          MVPRINTW(i + 15, j * 2 + 27, "  ");
        else if (game->next[i][j] == 1)
          MVPRINTW(i + 15, j * 2 + 27, "[]");
      }
    }
  }
}

void print_pause(bool pause) {
  if (pause) {
    MVPRINTW(19, 29, "PAUSE");
  } else
    MVPRINTW(19, 29, "     ");
}