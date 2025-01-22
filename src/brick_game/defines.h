#ifndef DEFINES_H
#define DEFINES_H

#define BOARDS_BEGIN 2
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define BOARD_N 20  // field length / rows
#define BOARD_M 10  // field width
#define HUD_WIDTH 12
#define EXIT_Q 113
#define START_S 115
#define SPACE 32
#define PAUSE_P 112
#define UP 0403
#define DOWN 0402
#define LEFT 0404
#define RIGHT 0405
#define LEVEL_CNT 10

#endif  // DEFINES_H
