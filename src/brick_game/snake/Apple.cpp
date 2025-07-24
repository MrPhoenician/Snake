#include "Apple.h"

using namespace s21;

[[nodiscard]] Apple::Coordinate Apple::getCoor() const { return coor; }

void Apple::setCoor(const int y, const int x) {
  coor.y = y;
  coor.x = x;
}

std::pair<int, int> Apple::generateApple() {
  thread_local std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<> distributionY(0, 19);
  std::uniform_int_distribution<> distributionX(0, 9);

  return {distributionY(generator), distributionX(generator)};
}
