#ifndef APPLE_H
#define APPLE_H

#include <random>

namespace s21 {

class Apple {
 public:
  typedef struct {
    int x;
    int y;
  } Coordinate;

  Apple() = default;
  [[nodiscard]] Coordinate getCoor() const;
  void setCoor(int y, int x);
  static std::pair<int, int> generateApple();

 private:
  Coordinate coor;
};

}  // namespace s21

#endif  // APPLE_H
