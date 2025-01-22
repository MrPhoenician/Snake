#ifndef API_H
#define API_H

typedef struct {
  int **field;  // игровое поле
  int **next;   // следующая фигура
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

// действия игрока
typedef enum {
  Start,
  Pause,
  Terminate,  // выход
  Left,
  Right,
  Up,
  Down,
  Action,  // вращение фигуры
} UserAction_t;

#ifdef __cplusplus
extern "C" {
#endif
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
#ifdef __cplusplus
}
#endif

#endif  // API_H
