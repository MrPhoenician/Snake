#include "tetris.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Game_t *game_ptr() {
  static Game_t game;

  return &game;
}

void initialize_structs(Game_t *game) {
  game->current = NULL;
  game->additional_field = NULL;
  game->stats.level = 1;
  game->stats.score = 0;
  game->stats.speed = 1;
  game->stats.pause = 0;
  game->stats.field = NULL;
  game->stats.next = NULL;
  game->state = Start_state;
  game->signal = 0;
  game->break_flag = true;
  game->init_flag = 0;
}

// проверка на невыделение памяти
int check_allocate(int error) {
  if (error == 1) return 1;

  return 0;
}

int allocate_matrix(int ***matrix, int rows, int columns) {
  int error = 0;
  if ((*matrix = (int **)calloc(
           1, rows * columns * sizeof(int) + rows * sizeof(int *))) == NULL) {
    error = 1;
  } else {
    (*matrix)[0] = (int *)(*matrix + rows);
    for (int i = 1; i < rows; i++) {
      (*matrix)[i] = (*matrix)[0] + (i * columns);
    }
  }

  return error;
}

void generate_fig(int **matrix, int *fig_type) {
  Game_t *game = game_ptr();
  static int counter = 0;
  srand(time(NULL) + counter++);
  *fig_type = rand() % 7;
  game->figure_pos.x = 7;
  game->figure_pos.y = 0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix[i][j] = 0;
    }
  }
  if (*fig_type == 0) {  // line
    matrix[1][0] = 1;
    matrix[1][1] = 1;
    matrix[1][2] = 1;
    matrix[1][3] = 1;
  } else if (*fig_type == 1) {  // square
    matrix[0][1] = 1;
    matrix[0][2] = 1;
    matrix[1][1] = 1;
    matrix[1][2] = 1;
  } else if (*fig_type == 2) {
    matrix[0][0] = 1;
    matrix[1][0] = 1;
    matrix[1][1] = 1;
    matrix[1][2] = 1;
  } else if (*fig_type == 3) {
    matrix[0][2] = 1;
    matrix[1][0] = 1;
    matrix[1][1] = 1;
    matrix[1][2] = 1;
  } else if (*fig_type == 4) {  // zigzag left
    matrix[0][1] = 1;
    matrix[0][2] = 1;
    matrix[1][0] = 1;
    matrix[1][1] = 1;
  } else if (*fig_type == 5) {  // zigzag right
    matrix[0][0] = 1;
    matrix[0][1] = 1;
    matrix[1][1] = 1;
    matrix[1][2] = 1;
  } else if (*fig_type == 6) {
    matrix[0][1] = 1;
    matrix[1][0] = 1;
    matrix[1][1] = 1;
    matrix[1][2] = 1;
  }
}

void generate_gameover(int **matrix) {
  matrix[0][1] = 1;
  matrix[1][1] = 1;
  matrix[2][1] = 1;
  matrix[3][1] = 1;
  matrix[3][6] = 1;
  matrix[3][7] = 1;
  matrix[3][8] = 1;
  matrix[4][1] = 1;
  matrix[4][2] = 1;
  matrix[4][3] = 1;
  matrix[4][6] = 1;
  matrix[4][8] = 1;
  matrix[5][6] = 1;
  matrix[5][8] = 1;
  matrix[6][6] = 1;
  matrix[6][8] = 1;
  matrix[7][2] = 1;
  matrix[7][6] = 1;
  matrix[7][7] = 1;
  matrix[7][8] = 1;
  matrix[8][1] = 1;
  matrix[8][3] = 1;
  matrix[9][1] = 1;
  matrix[9][3] = 1;
  matrix[10][1] = 1;
  matrix[11][2] = 1;
  matrix[12][2] = 1;
  matrix[13][3] = 1;
  matrix[14][3] = 1;
  matrix[15][1] = 1;
  matrix[15][3] = 1;
  matrix[15][6] = 1;
  matrix[15][7] = 1;
  matrix[15][8] = 1;
  matrix[16][2] = 1;
  matrix[14][1] = 1;
  matrix[16][6] = 1;
  matrix[17][6] = 1;
  matrix[17][7] = 1;
  matrix[18][6] = 1;
  matrix[19][6] = 1;
  matrix[19][7] = 1;
  matrix[19][8] = 1;
}

// сохранение фигуры на поле
void fill_field(Game_t *game) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->current[i][j] == 1) {
        game->stats.field[game->figure_pos.y + i][game->figure_pos.x + j - 4] =
            1;
      }
    }
  }
}

void fill_add_field(Game_t *game) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->current[i][j] == 1) {
        game->additional_field[game->figure_pos.y + i][game->figure_pos.x + j] =
            1;
      }
    }
  }
}

void merge_fields(Game_t *game) {
  for (int i = 0; i < BOARD_N; i++) {
    for (int j = 0; j < BOARD_M; j++) {
      game->stats.field[i][j] = game->additional_field[i][j + 4];
    }
  }
}

// удаляю снизу вверх и сдвигаю все поле вниз
int delete_row(Game_t *game) {
  int repeat;
  int count = 0;
  for (int i = BOARD_N - 1; i >= 0; i--) {
    repeat = 0;
    for (int j = 0; j < BOARD_M; j++) {
      if (game->stats.field[i][j] == 1) repeat++;
      if (repeat == 10) count++;
    }
  }
  repeat = count;
  while (repeat > 0) {
    for (int i = BOARD_N - 1; i >= 0; i--) {
      int flag = 0;
      for (int j = 0; j < BOARD_M; j++) {
        if (game->stats.field[i][j] == 1) {
          flag++;
        }
        if (flag == 10) {
          for (int x = 4; x < BOARD_M + 4; x++) {
            game->additional_field[i][x] = game->stats.field[i - 1][x - 4];
          }
          i -= 1;
          for (int r = i - 1; r >= 0; r--) {
            for (int l = 0; l < BOARD_M; l++) {
              game->additional_field[r + 1][l + 4] = game->stats.field[r][l];
            }
          }
        }
      }
    }
    repeat--;
    merge_fields(game);
  }
  if (count > 0) {
    for (int i = count - 1; i >= 0; i--) {
      for (int j = 4; j < BOARD_M + 4; j++) {
        game->additional_field[i][j] = 0;
      }
    }
  }
  return count;
}

void init_add_field(Game_t *game) {
  for (int i = 0; i < BOARD_N + 4; i++) {
    for (int j = 0; j < BOARD_M + 8; j++) {
      if ((j < 4 || j >= 14) && i <= 20)
        game->additional_field[i][j] = 2;
      else if (i >= 20)
        game->additional_field[i][j] = 2;
    }
  }
};

void free_matrix() {
  Game_t *game = game_ptr();
  if (game->current != NULL) {
    free(game->current);
    game->current = NULL;
  }
  if (game->stats.field != NULL) {
    free(game->stats.field);
    game->stats.field = NULL;
  }
  if (game->additional_field != NULL) {
    free(game->additional_field);
    game->additional_field = NULL;
  }
  if (game->stats.next != NULL) {
    free(game->stats.next);
    game->stats.next = NULL;
  }
}

void open_and_read() {
  Game_t *game = game_ptr();
  FILE *file = fopen("tetris.txt", "r");

  if (file) {
    if (fscanf(file, "%d", &game->stats.high_score) != 1) {
      game->state = Start_state;
      exitstate();
    }
    fclose(file);
  }
}

void open_and_write() {
  Game_t *game = game_ptr();
  FILE *file = fopen("tetris.txt", "r+");

  if (file) {
    fprintf(file, "%d", game->stats.score);
    fclose(file);
  } else {
    FILE *file = fopen("tetris.txt", "w");
    if (file) {
      fprintf(file, "%d", 0);
      fclose(file);
    }
  }
}

int allocate_game() {
  Game_t *game = game_ptr();
  int error = 1;
  if (check_allocate(allocate_matrix(&game->current, 4, 4)) == 0 &&
      check_allocate(allocate_matrix(&game->stats.field, BOARD_N, BOARD_M)) ==
          0 &&
      check_allocate(allocate_matrix(&game->additional_field, BOARD_N + 4,
                                     BOARD_M + 8)) == 0 &&
      check_allocate(allocate_matrix(&game->stats.next, 4, 4)) == 0) {
    init_add_field(game);
  } else {
    error = 0;
  }

  return error;
}

GameInfo_t updateCurrentState() {
  Game_t *game = game_ptr();

  if (game->stats.field == NULL && game->state != Exit &&
      game->break_flag == false) {
    if (game->state != Gameover) {
      initialize_structs(game);
    }
    if (!allocate_game()) {
      free_matrix();
    }
  }

  if (game->init_flag != 0 && game->state != Spawn) {
    merge_fields(game);
    fill_field(game);
  }

  if (game->state == Moving) {
    timer();
    if (game->timer.tt > game->timer.timer) {
      game->state = Shifting;
    }
  }

  if (game->user_act_mod == 8) {
    sigact(8);
  }

  game->user_act_mod = 8;

  return game->stats;
}

typedef void (*action_t)(Game_t *game);  // указатель на функцию
// using action = void (*)(Game_t *game); // тоже самое

void sigact(int action) {
  action_t fsm_table[8][9] = {
      {spawn, NULL, exitstate, NULL, NULL, NULL, NULL, NULL, start},
      {NULL, NULL, spawn, spawn, spawn, spawn, spawn, spawn, spawn},
      {NULL, pause, exitstate, moveleft, moveright, moveup, movedown, rotation,
       NULL},
      {shifting, shifting, shifting, shifting, shifting, shifting, shifting,
       shifting, shifting},
      {NULL, NULL, exitstate, attaching, attaching, attaching, attaching,
       attaching, attaching},
      {gameover, gameover, exitstate, gameover, gameover, gameover, gameover,
       gameover, gameover},
      {exitstate, exitstate, exitstate, exitstate, exitstate, exitstate,
       exitstate, exitstate, exitstate},
      {NULL, pause, exitstate, NULL, NULL, NULL, NULL, NULL, NULL}};

  Game_t *game = game_ptr();

  action_t act =
      fsm_table[game->state][action];  // текущий алгоритм выполнения КА

  if (act) act((Game_t *)game);  // act это колбэк на любую функцию
}

void userInput(UserAction_t action, bool hold) {
  Game_t *game = game_ptr();
  (void)hold;

  if (action >= 0 && action < 8) {
    sigact(action);
    game->user_act_mod = (int)action;
  }
}

int reach() {
  Game_t *game = game_ptr();
  int flag = 0;
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < BOARD_M + 8; j++) {
      if (game->stats.field[i][j] == 1) flag++;
    }
  }
  return flag;
}

int check_collide() {
  Game_t *game = game_ptr();
  int flag = 0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if ((game->current[i][j] *
           game->additional_field[game->figure_pos.y + i]
                                 [game->figure_pos.x + j]) > 0)
        flag++;
    }
  }
  return flag;
}

void start() { open_and_read(); }

void rotation() {
  Game_t *game = game_ptr();
  if (game->figure_pos.figure_type_cur != 1) {
    int row = 3;
    int col = 3;
    int size_mat = 3;
    if (game->figure_pos.figure_type_cur == 0) {
      row = 4;
      col = 4;
      size_mat = 4;
    }
    int temp_mat[row][col];
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        temp_mat[i][j] = game->current[i][j];
      }
    }
    for (int i = 0; i < row / 2; ++i) {
      for (int j = i; j < row - i - 1; ++j) {
        int temp = game->current[i][j];
        game->current[i][j] = game->current[size_mat - j - 1][i];
        game->current[size_mat - j - 1][i] =
            game->current[size_mat - i - 1][size_mat - j - 1];
        game->current[size_mat - i - 1][size_mat - j - 1] =
            game->current[j][size_mat - i - 1];
        game->current[j][size_mat - i - 1] = temp;
      }
    }
    if (check_collide() > 0) {
      for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
          game->current[i][j] = temp_mat[i][j];
        }
      }
    }
  }
  game->state = Moving;
}

void attaching() {
  Game_t *game = game_ptr();
  fill_add_field(game);
  fill_field(game);
  int count = delete_row(game);

  if (count == 1)
    game->stats.score += 100;
  else if (count == 2)
    game->stats.score += 300;
  else if (count == 3)
    game->stats.score += 700;
  else if (count == 4)
    game->stats.score += 1500;

  if (game->stats.score > game->stats.high_score) {
    open_and_write();
    game->stats.high_score = game->stats.score;
  }
  if (game->stats.score > game->stats.level * 600) {
    game->stats.level = game->stats.score / 600 + 1;
    game->stats.speed = game->stats.level;
  }

  game->state = Spawn;
}

void pause() {
  Game_t *game = game_ptr();
  if (game->state == Pause_state) {
    game->state = Moving;
    game->stats.pause = 0;
  } else {
    game->state = Pause_state;
    game->stats.pause = 1;
  }
}

void exitstate() {
  Game_t *game = game_ptr();
  if (game->state == Start_state) {
    game->break_flag = false;
    free_matrix();
    game->state = Exit;
  } else {
    game->state = Start_state;
    free_matrix();
    game->break_flag = false;
  }
}

void gameover() {
  Game_t *game = game_ptr();
  game->break_flag = false;
  free_matrix();

  if (allocate_game()) {
    if (game->state == Gameover) {
      generate_gameover(game->stats.field);
    }
  }
}

void shifting() {
  Game_t *game = game_ptr();

  game->figure_pos.y++;
  if (check_collide() > 0) {
    game->figure_pos.y--;
    game->state = Attaching;
  } else {
    game->state = Moving;
  }

  gettimeofday(&game->timer.mt1, NULL);
}

void moveleft() {
  Game_t *game = game_ptr();
  game->figure_pos.x--;
  if (check_collide() > 0) {
    game->figure_pos.x++;
  }
  game->state = Moving;
}

void moveright() {
  Game_t *game = game_ptr();
  game->figure_pos.x++;
  if (check_collide() > 0) {
    game->figure_pos.x--;
  }
  game->state = Moving;
}

void movedown() {
  Game_t *game = game_ptr();
  while (check_collide() == 0) {
    game->figure_pos.y++;
  }
  game->figure_pos.y--;
  game->state = Attaching;
}

void moveup() {}

void spawn() {
  Game_t *game = game_ptr();
  if (game->stats.level > LEVEL_CNT) game->state = Start_state;
  if (reach() > 0)
    game->state = Gameover;
  else {
    if (game->init_flag == 0) {
      generate_fig(game->current, &game->figure_pos.figure_type_cur);
      game->init_flag = 1;
      generate_fig(game->stats.next, &game->figure_pos.figure_type_next);
    } else {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          game->current[i][j] = game->stats.next[i][j];
          game->figure_pos.figure_type_cur = game->figure_pos.figure_type_next;
        }
      }
      generate_fig(game->stats.next, &game->figure_pos.figure_type_next);
    }
    game->state = Moving;
    gettimeofday(&game->timer.mt1, NULL);
  }
}

void timer() {
  Game_t *game = game_ptr();
  game->timer.timer = 1100 - ((100 * game->stats.level) - 100);
  gettimeofday(&game->timer.mt2, NULL);
  game->timer.tt = 1000 * (game->timer.mt2.tv_sec - game->timer.mt1.tv_sec) +
                   (game->timer.mt2.tv_usec - game->timer.mt1.tv_usec) / 1000;
}