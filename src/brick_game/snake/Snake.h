#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

#include "../api.h"

namespace s21 {

class Snake {
 public:
  Snake() { initSnake(); }
  void initSnake();
  void addSnakeHead();
  UserAction_t getDirection() const { return direction; }
  void setDirection(const UserAction_t value) { direction = value; }
  int getLength() const { return length; }
  void setLength(const int value) { length = value; }

  std::vector<std::pair<int, int>> field;

 private:
  UserAction_t direction;
  int length;
};

}  // namespace s21

#endif  // SNAKE_H
