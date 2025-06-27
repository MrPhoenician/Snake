cmake_minimum_required(VERSION 3.28)
project(CPP3_BrickGame_v2_0_1 C CXX)

# Найти Qt
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(CPP3_BrickGame_v2_0_1
        src/gui/cli/view.c
        src/brick_game/snake/Snake.cpp
        src/gui/desktop/main.cpp
        src/gui/desktop/widget.cpp
        src/gui/desktop/desktop.pro
        src/brick_game/tetris/tetris.c
        src/tests/test_main.cpp
        src/tests/test_snake.cpp
        src/brick_game/snake/Apple.cpp
        src/brick_game/snake/Game.cpp
)

target_link_libraries(CPP3_BrickGame_v2_0_1 ncurses)
target_link_libraries(CPP3_BrickGame_v2_0_1 Qt5::Widgets)
