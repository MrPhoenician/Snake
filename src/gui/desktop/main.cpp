#include <QApplication>

#include "widget.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Widget w;
  w.setStyleSheet("background-color: lightGrey;");
  w.show();

  return a.exec();
}
