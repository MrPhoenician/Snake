#ifndef WIDGET_H
#define WIDGET_H

#include <QKeyEvent>
// #include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QWidget>

#include "../../brick_game/api.h"
#include "../../brick_game/defines.h"
#define DELAY 20
#define SIZE_CELL 30

class Widget : public QWidget {
  Q_OBJECT

 private:
  int timer;
  double scaleX = 1.0;  // Коэффициент масштабирования по X
  double scaleY = 1.0;  // Коэффициент масштабирования по Y
  int xOffset = 0;  // Смещение по X
  int yOffset = 0;  // Смещение по Y
  void drawing();
  void drawing_text(QPainter *p, GameInfo_t const *game) const;

 protected:
  void timerEvent(QTimerEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

 public:
  explicit Widget(QWidget *parent = nullptr);
  ~Widget() override;
};
#endif  // WIDGET_H
