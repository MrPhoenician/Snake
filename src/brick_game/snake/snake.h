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
  void generateApple();

 private:
  Coordinate coor;
};

class Snake {
 public:
  Snake() { initSnake(); }

  std::vector<std::pair<int, int>> field;
  UserAction_t direction;
  int length;

  void initSnake();
  void addSnakeHead();
  void eating();
  void checkDirection() const;

 private:
};

class Game {
 public:
  Game();
  static Game *getGameObj();
  long myTimer();
  void setLevelAndSpeed();
  void sigact(int action);
  void writeToFile() const;
  void allocateMatr();
  void fillField();
  [[nodiscard]] int getField(int y, int x) const { return stats.field[y][x]; }
  void setField(int y, int x, int value) const { stats.field[y][x] = value; }
  static void destroyGameObj();
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

  ~Game() = default;

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
  void generateLose() const;
  void generateWin() const;
  void setZero() const;
  void freeField();
  void readFromFile();
  int checkCollide();
};

}  // namespace s21

#endif  // SNAKE_H
