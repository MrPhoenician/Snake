#ifndef VIEW_H
#define VIEW_H

#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../brick_game/api.h"

void print_overlay();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(GameInfo_t *game);
bool print_field(GameInfo_t *game);
void print_next(GameInfo_t *game);
void print_pause(bool pause);
void game_loop();
void get_signal(int userIn, UserAction_t *action);

#endif