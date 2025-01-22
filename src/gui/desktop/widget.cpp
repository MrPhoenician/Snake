#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent) {
  setGeometry(x(), y(), 550, 670);

#ifdef TETRIS
  setWindowTitle("Brickgame Tetris");
#else
  setWindowTitle("Brickgame Snake");
#endif

  timer = startTimer(DELAY);
}

void Widget::timerEvent(QTimerEvent *event) {
  Q_UNUSED(event);
  this->repaint();
}

void Widget::paintEvent(QPaintEvent *paint) {
  Q_UNUSED(paint);
  drawing();
}

void Widget::resizeEvent(QResizeEvent *event) {
  const int initialWidth = 550;
  const int initialHeight = 670;
  // текущий размер делим на начальный
  scaleX = static_cast<double>(width()) / initialWidth;
  scaleY = static_cast<double>(height()) / initialHeight;
  // Вычислим новое смещение для ячеек и текста
  xOffset = (width() - initialWidth * scaleX) / 2;
  yOffset = (height() - initialHeight * scaleY) / 2;

  QWidget::resizeEvent(event);
}

void Widget::keyPressEvent(QKeyEvent *event) {
  int key = event->key();

  if (key == Qt::Key_Up)
    userInput(Up, false);
  else if (key == Qt::Key_Down)
    userInput(Down, false);
  else if (key == Qt::Key_Left)
    userInput(Left, false);
  else if (key == Qt::Key_Right)
    userInput(Right, false);
  else if (key == Qt::Key_Q)
    userInput(Terminate, false);
  else if (key == Qt::Key_S)
    userInput(Start, false);
  else if (key == Qt::Key_Space)
    userInput(Action, false);
  else if (key == Qt::Key_P)
    userInput(Pause, false);
}

void Widget::drawing() {
  auto game = updateCurrentState();
  QPainter p(this);

  if (game.field == nullptr) {
    killTimer(timer);
    close();
  } else {
    for (int i = 0; i < BOARD_N; i++) {
      for (int j = 0; j < BOARD_M; j++) {
        if (game.field[i][j] == 1)
          p.setBrush(Qt::blue);
        else if (game.field[i][j] == 0) {
          p.setBrush(Qt::white);
        }
        p.drawRoundedRect((j + 1) * SIZE_CELL * scaleX + xOffset,
                          (i + 1) * SIZE_CELL * scaleY + yOffset,
                          SIZE_CELL * scaleX, SIZE_CELL * scaleY, 9 * scaleX,
                          9 * scaleY);
      }
    }
    drawing_text(&p, &game);
  }
  if (game.next != nullptr) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 4; j++) {
        if (game.next[i][j] == 1) {
          p.setBrush(Qt::green);
          p.drawRoundedRect((j + 13) * SIZE_CELL * scaleX + xOffset,
                            (i + 15) * SIZE_CELL * scaleY + yOffset,
                            SIZE_CELL * scaleX, SIZE_CELL * scaleY, 9 * scaleX,
                            9 * scaleY);
        }
      }
    }
  }
}

void Widget::drawing_text(QPainter *p, GameInfo_t const *game) const {
  QFont verdanaFont("Verdana",
                    std::min(static_cast<int>(14 * scaleX * scaleY), 26));
  p->setFont(verdanaFont);
  p->drawText(390 * scaleX + xOffset, 100 * scaleY + yOffset, "LEVEL");
  auto text = QString::number(game->level);
  p->drawText(480 * scaleX + xOffset, 100 * scaleY + yOffset, text);
  p->drawText(390 * scaleX + xOffset, 180 * scaleY + yOffset, "SCORE");
  text = QString::number(game->score);
  p->drawText(480 * scaleX + xOffset, 180 * scaleY + yOffset, text);
  p->drawText(390 * scaleX + xOffset, 260 * scaleY + yOffset, "HIGH");
  double verticalSpacing = 6 * scaleY * scaleX;
  p->drawText(390 * scaleX + xOffset, 280 * scaleY + yOffset + verticalSpacing,
              "SCORE");
  text = QString::number(game->high_score);
  p->drawText(480 * scaleX + xOffset, 272 * scaleY + yOffset, text);
  p->drawText(390 * scaleX + xOffset, 352 * scaleY + yOffset, "SPEED");
  text = QString::number(game->speed);
  p->drawText(480 * scaleX + xOffset, 352 * scaleY + yOffset, text);
  if (game->pause) {
    p->drawText(406 * scaleX + xOffset, 550 * scaleY + yOffset, "PAUSE");
  }
}

Widget::~Widget() = default;
