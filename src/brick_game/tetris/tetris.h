#ifndef C7_BRICKGAME_V1_0_1_BACKEND_H
#define C7_BRICKGAME_V1_0_1_BACKEND_H

#define LEVEL_CNT 10

#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "../api.h"

typedef struct {
  int x;
  int y;
  int figure_type_cur;
  int figure_type_next;
} Figure_pos_t;

// КА
typedef enum {
  Start_state = 0,
  Spawn,
  Moving,  // вращение фигуры / движение по горизонтали
  Shifting,  // смещение вниз
  Attaching,
  Gameover,
  Exit,
  Pause_state
} GameState_t;

typedef struct {
  struct timeval mt1, mt2;
  long long tt;
  long long timer;
} Timer_t;

typedef struct {
  int **current;
  int **additional_field;
  GameState_t state;
  UserAction_t user_act;
  GameInfo_t stats;
  Figure_pos_t figure_pos;
  Timer_t timer;
  int signal;
  bool break_flag;
  int init_flag;
  int no_action_flag;
  int user_act_mod;
} Game_t;

int allocate_matrix(int ***matrix, int rows, int columns);
int allocate_game();
void free_matrix();
int check_allocate(int error);
void generate_fig(int **matrix, int *fig_type);
void fill_field(const Game_t *game);
void initialize_structs(Game_t *game);
void sigact(int action);
Game_t *game_ptr();
void init_add_field(const Game_t *game);
int check_collide();
void merge_fields(const Game_t *game);
int delete_row(const Game_t *game);
void timer();
void open_and_read();
void open_and_write();
void generate_gameover(int **matrix);

void start();
void spawn();
void moveup();
void movedown();
void moveright();
void moveleft();
void shifting();
void gameover();
void exitstate();
void pause();
void rotation();
void attaching();

#endif  // C7_BRICKGAME_V1_0_1_BACKEND_H
