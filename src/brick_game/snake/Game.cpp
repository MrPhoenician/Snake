#include "Game.h"

using namespace s21;

Game *Game::gameObj = nullptr;

Game *Game::getGameObj() {
  if (gameObj == nullptr) {
    gameObj = new Game();
  }

  return gameObj;
}

typedef void (Game::*action_t)();

void Game::sigact(const int action) {
  const action_t fsm_table[6][9] = {
      {&Game::spawn, nullptr, &Game::exitstate, nullptr, nullptr, nullptr,
       nullptr, nullptr, &Game::start},
      {nullptr, &Game::pause, &Game::exitstate, nullptr, nullptr, nullptr,
       nullptr, nullptr, nullptr},
      {&Game::shifting, &Game::shifting, &Game::shifting, &Game::shifting,
       &Game::shifting, &Game::shifting, &Game::shifting, &Game::shifting,
       &Game::shifting},
      {&Game::gameover, &Game::gameover, &Game::exitstate, &Game::gameover,
       &Game::gameover, &Game::gameover, &Game::gameover, &Game::gameover,
       &Game::gameover},
      {&Game::exitstate, &Game::exitstate, &Game::exitstate, &Game::exitstate,
       &Game::exitstate, &Game::exitstate, &Game::exitstate, &Game::exitstate,
       &Game::exitstate},
      {nullptr, &Game::pause, &Game::exitstate, nullptr, nullptr, nullptr,
       nullptr, nullptr, nullptr}};

  if (const action_t act = fsm_table[state][action]) {
    (this->*act)();
  }
}

int Game::checkCollide() const {
  int error = 0;
  int x = snake.field.back().second;
  int y = snake.field.back().first;

  if (snake.getDirection() == Right) {
    x += 1;
  } else if (snake.getDirection() == Left) {
    x -= 1;
  } else if (snake.getDirection() == Up) {
    y -= 1;
  } else if (snake.getDirection() == Down) {
    y += 1;
  }

  if (x != BOARD_M && x != -1 && y != BOARD_N && y != -1) {
    if ((stats.field[y][x] == 1 &&
         (y != apple.getCoor().y && x != apple.getCoor().x))) {
      error = 1;
    }
  } else {
    error = 1;
  }

  return error;
}

long Game::myTimer() {
  end_t = steady_clock::now();
  const auto difference = duration_cast<milliseconds>(end_t - begin_t).count();

  return difference;
}

void userInput(UserAction_t action, [[maybe_unused]] bool hold) {
  const auto game = Game::getGameObj();

  if (action >= 0 && action < 8) {
    game->sigact(action);
    game->userAct = action;
    game->userActMod = action;
  }
}

GameInfo_t updateCurrentState() {
  const auto game = Game::getGameObj();

  if (game->stats.field == nullptr && game->state != Exit) {
    game->allocateMatr();
  } else if (game->state != Exit && game->state != Start_state &&
             game->state != Pause_state && game->state != Gameover) {
    game->fillField();
  }

  if (game->state == Exit) {
    Game::destroyGameObj();
    constexpr GameInfo_t empty = {};

    return empty;
  }

  if (game->state == Moving && game->userAct == Action) {
    if (game->myTimer() > game->maxSpeed) {
      game->state = Shifting;
      game->userAct = game->snake.getDirection();
    }
  } else if (game->state == Moving) {
    if (game->myTimer() > game->speed) {
      game->state = Shifting;
    }
  }

  if (game->userActMod == 8) {
    game->sigact(8);
  }

  game->userActMod = 8;

  return game->stats;
}

void Game::shifting() {
  checkDirection();

  if (userAct == Up || userAct == Down || userAct == Right || userAct == Left) {
    snake.setDirection(userAct);
  }

  if (!checkCollide()) {
    snake.addSnakeHead();

    if ((snake.field.back().first == apple.getCoor().y) &&
        (snake.field.back().second == apple.getCoor().x)) {
      eating();
    } else {
      stats.field[snake.field[0].first][snake.field[0].second] = 0;
      snake.field.erase(snake.field.begin());
      state = Moving;
      begin_t = steady_clock::now();
    }
  } else {
    state = Gameover;
    loseFlag = true;
  }
}

void Game::gameover() {
  if (loseFlag) {
    setZero();
    snake.field.clear();
    generateLose();
    loseFlag = false;
  } else if (winFlag) {
    setZero();
    snake.field.clear();
    generateWin();
    winFlag = false;
  }
}

void Game::pause() {
  if (state != Pause_state) {
    state = Pause_state;
    stats.pause = true;
  } else {
    userAct = snake.getDirection();
    state = Moving;
    stats.pause = false;
    begin_t = steady_clock::now();
  }
}

void Game::exitstate() {
  freeField();

  if (state == Start_state) {
    state = Exit;
  } else {
    snake.field.clear();
    state = Start_state;
  }
}

void Game::start() {
  stats.score = 0;
  stats.level = 1;
  stats.speed = 1;
  speed = 450;
  maxSpeed = 20;
  loseFlag = false;
  winFlag = false;
  stats.pause = false;
  snake.initSnake();
  readFromFile();
}

void Game::spawn() {
  if (state != Exit) {
    for (auto [first, second] : snake.field) {
      stats.field[first][second] = 1;
    }

    const auto game = getGameObj();
    int y, x;

    do {
      std::tie(y, x) = Apple::generateApple();
    } while (game->getField(y, x) == 1);

    game->apple.setCoor(y, x);
    game->setField(y, x, 1);

    state = Shifting;
    userAct = Right;
    snake.setDirection(Right);
  }
}

void Game::allocateMatr() {
  stats.field = new int *[BOARD_N];
  for (int i = 0; i < BOARD_N; i++) {
    stats.field[i] = new int[BOARD_M]{};
  }
}

void Game::freeField() {
  if (stats.field != nullptr) {
    for (int i = 0; i < BOARD_N; i++) {
      delete[] stats.field[i];
    }
    delete[] stats.field;
    stats.field = nullptr;
  }
}

void Game::readFromFile() {
  if (std::ifstream file{"snake.txt"}; !file.is_open()) {
  } else {
    std::string score;

    while (std::getline(file, score)) {
      stats.high_score = std::stoi(score);
    }
    file.close();
  }
}

void Game::writeToFile() const {
  if (std::ofstream file{"snake.txt"}; !file) {
  } else {
    file << stats.high_score;
    file.close();
  }
}

void Game::fillField() const {
  for (auto &i : snake.field) {
    stats.field[i.first][i.second] = 1;
  }
}

void Game::setZero() const {
  for (int i = 0; i < BOARD_N; i++) {
    for (int j = 0; j < BOARD_M; j++) {
      setField(i, j, 0);
    }
  }
}

void Game::setLevelAndSpeed() {
  if (stats.level <= 10) {
    stats.level += 1;
    stats.speed += 1;
    speed -= 35;
  }
}

Game::Game() {
  userActMod = 0;
  stats.field = nullptr;
  stats.next = nullptr;
  state = Start_state;
}

void Game::destroyGameObj() {
  delete gameObj;
  gameObj = nullptr;
}

void Game::checkDirection() const {
  const auto game = getGameObj();

  if ((snake.getDirection() == Up && game->userAct == Down) ||
      (snake.getDirection() == Down && game->userAct == Up) ||
      (snake.getDirection() == Right && game->userAct == Left) ||
      (snake.getDirection() == Left && game->userAct == Right)) {
    game->userAct = snake.getDirection();
  }
}

void Game::eating() {
  const auto game = Game::getGameObj();
  snake.setLength(snake.getLength() + 1);
  game->stats.score += 1;

  if (game->stats.score % 5 == 0) {
    game->setLevelAndSpeed();
  }

  if (game->stats.score > game->stats.high_score) {
    game->stats.high_score = game->stats.score;
    game->writeToFile();
  }

  if (snake.getLength() == 200) {
    game->state = Gameover;
    game->winFlag = true;
  } else {
    int y, x;
    do {
      std::tie(y, x) = Apple::generateApple();
    } while (game->getField(y, x) == 1);

    game->apple.setCoor(y, x);
    game->setField(y, x, 1);
  }
}

int Game::getField(const int y, const int x) const { return stats.field[y][x]; }
void Game::setField(const int y, const int x, const int value) const {
  stats.field[y][x] = value;
}

void Game::generateLose() const {
  stats.field[0][1] = 1;
  stats.field[1][1] = 1;
  stats.field[2][1] = 1;
  stats.field[3][1] = 1;
  stats.field[3][6] = 1;
  stats.field[3][7] = 1;
  stats.field[3][8] = 1;
  stats.field[4][1] = 1;
  stats.field[4][2] = 1;
  stats.field[4][3] = 1;
  stats.field[4][6] = 1;
  stats.field[4][8] = 1;
  stats.field[5][6] = 1;
  stats.field[5][8] = 1;
  stats.field[6][6] = 1;
  stats.field[6][8] = 1;
  stats.field[7][2] = 1;
  stats.field[7][6] = 1;
  stats.field[7][7] = 1;
  stats.field[7][8] = 1;
  stats.field[8][1] = 1;
  stats.field[8][3] = 1;
  stats.field[9][1] = 1;
  stats.field[9][3] = 1;
  stats.field[10][1] = 1;
  stats.field[11][2] = 1;
  stats.field[12][2] = 1;
  stats.field[13][3] = 1;
  stats.field[14][3] = 1;
  stats.field[15][1] = 1;
  stats.field[15][3] = 1;
  stats.field[15][6] = 1;
  stats.field[15][7] = 1;
  stats.field[15][8] = 1;
  stats.field[16][2] = 1;
  stats.field[14][1] = 1;
  stats.field[16][6] = 1;
  stats.field[17][6] = 1;
  stats.field[17][7] = 1;
  stats.field[18][6] = 1;
  stats.field[19][6] = 1;
  stats.field[19][7] = 1;
  stats.field[19][8] = 1;
}

void Game::generateWin() const {
  stats.field[0][0] = 1;
  stats.field[1][0] = 1;
  stats.field[2][0] = 1;
  stats.field[3][0] = 1;
  stats.field[4][0] = 1;
  stats.field[5][0] = 1;
  stats.field[6][1] = 1;
  stats.field[5][2] = 1;
  stats.field[4][2] = 1;
  stats.field[3][2] = 1;
  stats.field[2][3] = 1;
  stats.field[3][4] = 1;
  stats.field[4][4] = 1;
  stats.field[5][4] = 1;
  stats.field[6][5] = 1;
  stats.field[5][6] = 1;
  stats.field[4][6] = 1;
  stats.field[3][6] = 1;
  stats.field[2][6] = 1;
  stats.field[1][6] = 1;
  stats.field[0][6] = 1;
  stats.field[5][9] = 1;
  stats.field[6][9] = 1;
  stats.field[7][9] = 1;
  stats.field[8][9] = 1;
  stats.field[9][9] = 1;
  stats.field[10][9] = 1;
  stats.field[11][9] = 1;
  stats.field[12][9] = 1;
  stats.field[13][0] = 1;
  stats.field[14][0] = 1;
  stats.field[15][0] = 1;
  stats.field[16][0] = 1;
  stats.field[17][0] = 1;
  stats.field[18][0] = 1;
  stats.field[19][0] = 1;
  stats.field[13][1] = 1;
  stats.field[14][2] = 1;
  stats.field[15][2] = 1;
  stats.field[16][2] = 1;
  stats.field[17][3] = 1;
  stats.field[18][3] = 1;
  stats.field[19][4] = 1;
  stats.field[19][5] = 1;
  stats.field[18][5] = 1;
  stats.field[17][5] = 1;
  stats.field[16][5] = 1;
  stats.field[15][5] = 1;
  stats.field[14][5] = 1;
  stats.field[13][5] = 1;
}