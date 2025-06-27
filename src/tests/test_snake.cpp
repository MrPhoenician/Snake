#include "test_main.h"

using namespace s21;

TEST(test_snake, snake_1) {
  const auto gameObj = Game::getGameObj();
  auto game = updateCurrentState();
  gameObj->userAct = Start;
  userInput(gameObj->userAct, false);
  game = updateCurrentState();
  gameObj->userAct = Pause;
  userInput(gameObj->userAct, false);
  game = updateCurrentState();
  gameObj->userAct = Pause;
  userInput(gameObj->userAct, false);
  game = updateCurrentState();
  gameObj->apple.setCoor(10, 7);
  gameObj->setField(10, 7, 1);
  gameObj->userAct = Action;
  gameObj->state = Moving;
  userInput(gameObj->userAct, false);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  game = updateCurrentState();
  gameObj->userAct = Right;
  game = updateCurrentState();
  auto [y, x] = Apple::generateApple();
  gameObj->apple.setCoor(y, x);
  gameObj->setField(y, x, 1);
  gameObj->stats.score = 4;
  gameObj->eating();
  gameObj->snake.setLength(199);
  gameObj->eating();
  gameObj->state = Gameover;
  gameObj->loseFlag = true;
  game = updateCurrentState();
  gameObj->state = Gameover;
  gameObj->winFlag = true;
  game = updateCurrentState();
  gameObj->userAct = Terminate;
  userInput(gameObj->userAct, false);
  game = updateCurrentState();
  gameObj->userAct = Terminate;
  userInput(gameObj->userAct, false);
  game = updateCurrentState();
}

TEST(test_snake, snake_2) {
  const auto gameObj = Game::getGameObj();
  auto game = updateCurrentState();
  gameObj->userAct = Start;
  userInput(gameObj->userAct, false);
  game = updateCurrentState();
  gameObj->stats.score = 8;
  gameObj->stats.level = 7;
  gameObj->stats.speed = 6;
  gameObj->speed = 100;
  game = updateCurrentState();
  EXPECT_EQ(gameObj->stats.score, 8);
  EXPECT_EQ(gameObj->stats.level, 7);
  EXPECT_EQ(gameObj->stats.speed, 6);
  EXPECT_EQ(gameObj->speed, 100);
}