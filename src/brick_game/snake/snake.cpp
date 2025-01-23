#include "snake.h"
using namespace s21;

Game *Game::gameObj = nullptr;

Game *Game::getGameObj() {
  if (gameObj == nullptr) {
    gameObj = new Game();
  }

  return gameObj;
}

typedef void (Game::*action_t)();

void Game::sigact(int action) {
  action_t fsm_table[6][9] = {
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

  action_t act = fsm_table[this->state][action];

  if (act) {
    (this->*act)();
  }
}

int Game::check_collide() {
  int error = 0;
  int x = snake.field.back().second;
  int y = snake.field.back().first;

  if (snake.direction == Right) {
    x += 1;
  } else if (snake.direction == Left) {
    x -= 1;
  } else if (snake.direction == Up) {
    y -= 1;
  } else if (snake.direction == Down) {
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

long Game::my_timer() {
  end_t = steady_clock::now();
  auto difference = duration_cast<milliseconds>(end_t - begin_t).count();

  return difference;
}

void userInput(UserAction_t action, [[maybe_unused]] bool hold) {
  auto game = Game::getGameObj();

  if (action >= 0 && action < 8) {
    game->sigact(action);
    game->userAct = action;
    game->userActMod = action;
  }
}

GameInfo_t updateCurrentState() {
  auto game = Game::getGameObj();

  if (game->stats.field == nullptr && game->state != Exit) {
    game->allocate_matr();
  } else if (game->state != Exit && game->state != Start_state &&
             game->state != Pause_state && game->state != Gameover) {
    game->fill_field();
  }

  if (game->state == Moving && game->userAct == Action) {
    if (game->my_timer() > game->maxSpeed) {
      game->state = Shifting;
      game->userAct = game->snake.direction;
    }
  } else if (game->state == Moving) {
    if (game->my_timer() > game->speed) {
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
  snake.check_direction();

  if (this->userAct == Up || this->userAct == Down || this->userAct == Right ||
      this->userAct == Left) {
    this->snake.direction = this->userAct;
  }

  if (!this->check_collide()) {
    this->snake.add_snake_head();

    if ((this->snake.field.back().first == this->apple.getCoor().y) &&
        (this->snake.field.back().second == this->apple.getCoor().x)) {
      this->snake.eating();
    } else {
      this->stats
          .field[this->snake.field[0].first][this->snake.field[0].second] = 0;
      this->snake.field.erase(this->snake.field.begin());
      this->state = Moving;
      this->begin_t = steady_clock::now();
    }
  } else {
    this->state = Gameover;
    this->loseFlag = true;
  }
}

void Game::gameover() {
  if (this->loseFlag) {
    set_zero();
    this->snake.field.clear();
    generate_lose();
    this->loseFlag = false;
  } else if (this->winFlag) {
    set_zero();
    this->snake.field.clear();
    generate_win();
    this->winFlag = false;
  }
}

void Game::pause() {
  if (this->state != Pause_state) {
    this->state = Pause_state;
    this->stats.pause = true;
  } else {
    this->userAct = this->snake.direction;
    this->state = Moving;
    this->stats.pause = false;
    this->begin_t = steady_clock::now();
  }
}

void Game::exitstate() {
  if (this->state == Start_state) {
    free_field();
    this->state = Exit;
  } else {
    free_field();
    this->snake.field.clear();
    this->state = Start_state;
  }
}

void Game::start() {
  this->stats.score = 0;
  this->stats.level = 1;
  this->stats.speed = 1;
  this->speed = 450;
  this->maxSpeed = 20;
  this->loseFlag = false;
  this->winFlag = false;
  this->stats.pause = false;
  this->snake.init_snake();
  read_from_file();
}

void Game::spawn() {
  if (this->state != Exit) {
    this->stats.field[this->apple.getCoor().y][this->apple.getCoor().x] = 1;

    for (auto i : this->snake.field) {
      this->stats.field[i.first][i.second] = 1;
    }

    this->state = Shifting;
    this->userAct = Right;
    this->snake.direction = Right;
  }
}

void Game::allocate_matr() {
  this->stats.field = new int *[BOARD_N];
  for (int i = 0; i < BOARD_N; i++) {
    this->stats.field[i] = new int[BOARD_M];
  }
  for (int i = 0; i < BOARD_N; i++) {
    for (int j = 0; j < BOARD_M; j++) {
      this->stats.field[i][j] = 0;
    }
  }
}

void Game::free_field() {
  if (this->stats.field != nullptr) {
    for (int i = 0; i < BOARD_N; i++) {
      delete[] this->stats.field[i];
    }
    delete[] this->stats.field;
    this->stats.field = nullptr;
  }
}

void Apple::generate_apple() {
  auto game = Game::getGameObj();
  int y, x;
  do {
    srand(time(nullptr));
    y = rand() % 20;
    x = rand() % 10;
  } while (game->getField(y, x) == 1);

  setCoor(y, x);
  game->setField(y, x, 1);
}

void Game::read_from_file() {
  std::ifstream file{"snake.txt"};

  if (!file.is_open()) {
  } else {
    std::string score;

    while (std::getline(file, score)) {
      this->stats.high_score = std::stoi(score);
    }
    file.close();
  }
}

void Game::write_to_file() const {
  std::ofstream file{"snake.txt"};

  if (!file) {
  } else {
    file << this->stats.high_score;
    file.close();
  }
}

void Game::fill_field() {
  for (auto &i : this->snake.field) {
    this->stats.field[i.first][i.second] = 1;
  }
}

void Game::set_zero() const {
  for (int i = 0; i < BOARD_N; i++) {
    for (int j = 0; j < BOARD_M; j++) {
      this->setField(i, j, 0);
    }
  }
}

void Snake::check_direction() const {
  auto game = Game::getGameObj();
  if ((this->direction == Up && game->userAct == Down) ||
      (this->direction == Down && game->userAct == Up)) {
    game->userAct = this->direction;
  } else if ((this->direction == Right && game->userAct == Left) ||
             (this->direction == Left && game->userAct == Right)) {
    game->userAct = this->direction;
  }
}

void Snake::add_snake_head() {
  int x = this->field.back().second;
  int y = this->field.back().first;

  if (this->direction == Up) {
    y -= 1;
  } else if (this->direction == Right) {
    x += 1;
  } else if (this->direction == Left) {
    x -= 1;
  } else if (this->direction == Down) {
    y += 1;
  }

  this->field.emplace_back(y, x);
}

void Snake::init_snake() {
  field.resize(4);
  field[0] = {10, 3};
  field[1] = {10, 4};
  field[2] = {10, 5};
  field[3] = {10, 6};
  length = 4;
}

void Snake::eating() {
  auto game = Game::getGameObj();
  this->length += 1;
  game->stats.score += 1;

  if (game->stats.score % 5 == 0) {
    game->setLevelAndSpeed();
  }

  if (game->stats.score > game->stats.high_score) {
    game->stats.high_score = game->stats.score;
    game->write_to_file();
  }

  if (this->length == 200) {
    game->state = Gameover;
    game->winFlag = true;
  } else {
    game->apple.generate_apple();
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

void Game::generate_lose() const {
  this->stats.field[0][1] = 1;
  this->stats.field[1][1] = 1;
  this->stats.field[2][1] = 1;
  this->stats.field[3][1] = 1;
  this->stats.field[3][6] = 1;
  this->stats.field[3][7] = 1;
  this->stats.field[3][8] = 1;
  this->stats.field[4][1] = 1;
  this->stats.field[4][2] = 1;
  this->stats.field[4][3] = 1;
  this->stats.field[4][6] = 1;
  this->stats.field[4][8] = 1;
  this->stats.field[5][6] = 1;
  this->stats.field[5][8] = 1;
  this->stats.field[6][6] = 1;
  this->stats.field[6][8] = 1;
  this->stats.field[7][2] = 1;
  this->stats.field[7][6] = 1;
  this->stats.field[7][7] = 1;
  this->stats.field[7][8] = 1;
  this->stats.field[8][1] = 1;
  this->stats.field[8][3] = 1;
  this->stats.field[9][1] = 1;
  this->stats.field[9][3] = 1;
  this->stats.field[10][1] = 1;
  this->stats.field[11][2] = 1;
  this->stats.field[12][2] = 1;
  this->stats.field[13][3] = 1;
  this->stats.field[14][3] = 1;
  this->stats.field[15][1] = 1;
  this->stats.field[15][3] = 1;
  this->stats.field[15][6] = 1;
  this->stats.field[15][7] = 1;
  this->stats.field[15][8] = 1;
  this->stats.field[16][2] = 1;
  this->stats.field[14][1] = 1;
  this->stats.field[16][6] = 1;
  this->stats.field[17][6] = 1;
  this->stats.field[17][7] = 1;
  this->stats.field[18][6] = 1;
  this->stats.field[19][6] = 1;
  this->stats.field[19][7] = 1;
  this->stats.field[19][8] = 1;
}

void Game::generate_win() const {
  this->stats.field[0][0] = 1;
  this->stats.field[1][0] = 1;
  this->stats.field[2][0] = 1;
  this->stats.field[3][0] = 1;
  this->stats.field[4][0] = 1;
  this->stats.field[5][0] = 1;
  this->stats.field[6][1] = 1;
  this->stats.field[5][2] = 1;
  this->stats.field[4][2] = 1;
  this->stats.field[3][2] = 1;
  this->stats.field[2][3] = 1;
  this->stats.field[3][4] = 1;
  this->stats.field[4][4] = 1;
  this->stats.field[5][4] = 1;
  this->stats.field[6][5] = 1;
  this->stats.field[5][6] = 1;
  this->stats.field[4][6] = 1;
  this->stats.field[3][6] = 1;
  this->stats.field[2][6] = 1;
  this->stats.field[1][6] = 1;
  this->stats.field[0][6] = 1;
  this->stats.field[5][9] = 1;
  this->stats.field[6][9] = 1;
  this->stats.field[7][9] = 1;
  this->stats.field[8][9] = 1;
  this->stats.field[9][9] = 1;
  this->stats.field[10][9] = 1;
  this->stats.field[11][9] = 1;
  this->stats.field[12][9] = 1;
  this->stats.field[13][0] = 1;
  this->stats.field[14][0] = 1;
  this->stats.field[15][0] = 1;
  this->stats.field[16][0] = 1;
  this->stats.field[17][0] = 1;
  this->stats.field[18][0] = 1;
  this->stats.field[19][0] = 1;
  this->stats.field[13][1] = 1;
  this->stats.field[14][2] = 1;
  this->stats.field[15][2] = 1;
  this->stats.field[16][2] = 1;
  this->stats.field[17][3] = 1;
  this->stats.field[18][3] = 1;
  this->stats.field[19][4] = 1;
  this->stats.field[19][5] = 1;
  this->stats.field[18][5] = 1;
  this->stats.field[17][5] = 1;
  this->stats.field[16][5] = 1;
  this->stats.field[15][5] = 1;
  this->stats.field[14][5] = 1;
  this->stats.field[13][5] = 1;
}