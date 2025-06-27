#include "Snake.h"

using namespace s21;

void Snake::addSnakeHead() {
  int x = field.back().second;
  int y = field.back().first;

  if (direction == Up) {
    y -= 1;
  } else if (direction == Right) {
    x += 1;
  } else if (direction == Left) {
    x -= 1;
  } else if (direction == Down) {
    y += 1;
  }

  field.emplace_back(y, x);
}

void Snake::initSnake() {
  field.resize(4);
  field[0] = {10, 3};
  field[1] = {10, 4};
  field[2] = {10, 5};
  field[3] = {10, 6};
  length = 4;
}