QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

contains(DEFINES, TETRIS) {
TARGET = tetris_desk
SOURCES += \
    main.cpp \
    widget.cpp \
    ../../brick_game/tetris/tetris.c

HEADERS += \
    widget.h \
    ../../brick_game/api.h \
    ../../brick_game/tetris/tetris.h
} else {
TARGET = snake_desk

SOURCES += \
    main.cpp \
    widget.cpp \
    ../../brick_game/snake/Snake.cpp \
    ../../brick_game/snake/Apple.cpp \
    ../../brick_game/snake/Game.cpp

HEADERS += \
    widget.h \
    ../../brick_game/api.h \
    ../../brick_game/snake/Snake.h \
    ../../brick_game/snake/Apple.h \
    ../../brick_game/snake/Game.h
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CFLAGS += -Wall -Werror -Wextra
QMAKE_CXXFLAGS += -Wall -Werror -Wextra
