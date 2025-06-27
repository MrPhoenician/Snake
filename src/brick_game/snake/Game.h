#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <fstream>

#include "../api.h"
#include "Apple.h"
#include "Snake.h"

namespace s21 {
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

class Game {
 public:
  Game();
  static Game *getGameObj();
  long myTimer();
  void setLevelAndSpeed();
  void sigact(int action);
  void writeToFile() const;
  void allocateMatr();
  void fillField() const;
  [[nodiscard]] int getField(int y, int x) const;
  void setField(int y, int x, int value) const;
  static void destroyGameObj();
  void checkDirection() const;
  void eating();
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
  int checkCollide() const;

  static Game *gameObj;
  time_point<steady_clock, duration<double>> end_t;
  time_point<steady_clock, duration<double>> begin_t;
};
}  // namespace s21

#endif  // GAME_H
