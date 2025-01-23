#ifndef SNAKE_H
#define SNAKE_H

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../api.h"

using namespace std::chrono;

// FSM
typedef enum {
  Start_state = 0,
  Moving,
  Shifting,
  Gameover,
  Exit,
  Pause_state
} GameState_t;

typedef struct {
  int x;
  int y;
} Coordinate;

namespace s21 {

class Game;

class Apple {
 public:
  Apple() : coor({5, 5}) {}

  [[nodiscard]] Coordinate getCoor() const { return coor; }
  void setCoor(int y, int x) {
    coor.y = y;
    coor.x = x;
  }
  void generate_apple();

 private:
  Coordinate coor;
};

class Snake {
 public:
  Snake() { init_snake(); }

  std::vector<std::pair<int, int>> field;
  UserAction_t direction;
  int length;

  void init_snake();
  void add_snake_head();
  void eating();
  void check_direction() const;

 private:
};

class Game {
 public:
  Game();

  GameInfo_t stats;
  UserAction_t userAct;
  GameState_t state;
  bool winFlag;
  bool loseFlag;
  Snake snake;
  Apple apple;
  int speed;
  int maxSpeed;
  int userActMod;
  static Game *getGameObj();
  long my_timer();
  void setLevelAndSpeed();
  void sigact(int action);
  void write_to_file() const;
  void allocate_matr();
  void fill_field();
  [[nodiscard]] int getField(int y, int x) const { return stats.field[y][x]; }
  void setField(int y, int x, int value) const { stats.field[y][x] = value; }

  ~Game() {
    delete gameObj;
    gameObj = nullptr;
  }

 private:
  static Game *gameObj;
  time_point<steady_clock, duration<double>> end_t;
  time_point<steady_clock, duration<double>> begin_t;
  void shifting();
  void start();
  void spawn();
  void gameover();
  void exitstate();
  void pause();
  void generate_lose() const;
  void generate_win() const;
  void set_zero() const;
  void free_field();
  void read_from_file();
  int check_collide();
};

}  // namespace s21

#endif  // SNAKE_H
