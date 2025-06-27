#ifndef VIEW_H
#define VIEW_H

#define BOARDS_BEGIN 2
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define HUD_WIDTH 12
#define EXIT_Q 113
#define START_S 115
#define SPACE 32
#define PAUSE_P 112
#define UP 0403
#define DOWN 0402
#define LEFT 0404
#define RIGHT 0405

#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../brick_game/api.h"

void print_overlay();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(const GameInfo_t *game);
bool print_field(const GameInfo_t *game);
void print_next(const GameInfo_t *game);
void print_pause(bool pause);
void game_loop();
void get_signal(int userIn, UserAction_t *action);

#endif