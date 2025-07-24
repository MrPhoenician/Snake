SHELL = /bin/sh

CC := gcc
CXX := g++
CFLAGS := -std=c11 -Wall -Wextra -Werror
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror
OBJS := out/view.o out/Snake.o out/Apple.o out/Game.o
OBJS_C := out/view_c.o out/tetris.o
GCOV := --coverage
T1 := snake_cli
T2 := snake_desk
T3 := tetris_cli
T4 := tetris_desk

# make dist variable
PACKAGE := snake
VERSION := 1.0
TARFILE := $(PACKAGE)_$(VERSION)
BINDIR := ./bindir
DOCDIR := ./docdir
SOURCES := ./image ./out/*.o
DOCPAGES := ./dvi

.PHONY: all install uninstall clean dvi dist test valgrind clang gcov_report

all: $(OBJS_C) $(OBJS) $(T1) $(T2) $(T3) $(T4)

$(T1):
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(T1) -lncurses

$(T2):
	cd gui/desktop && qmake desktop.pro && make && make clean && rm Makefile .qmake.stash && mv snake_desk ../../

$(T3):
	$(CC) $(CFLAGS) $(OBJS_C) -o $(T3) -lncurses

$(T4):
	cd gui/desktop && qmake desktop.pro "DEFINES += TETRIS" && make && make clean && rm Makefile .qmake.stash && mv tetris_desk ../../

install: $(T2)
	mkdir -p ~/brickgame_by_harkonex/
	install ./snake_desk ~/brickgame_by_harkonex/

uninstall:
	rm -f ~/brickgame_by_harkonex/snake_desk
	rmdir ~/brickgame_by_harkonex/

test:
	$(CXX) $(CXXFLAGS) brick_game/snake/*cpp tests/*.cpp -o test -lgtest
	./test

gcov_report: ext_flag test
	-rm -rf report/
	-lcov --rc branch_coverage=1 --ignore-errors mismatch --rc geninfo_unexecuted_blocks=1 -t "s21_snake_report" -o s21_snake_report_all.info -c -d .
	-lcov --rc branch_coverage=1 -r s21_snake_report_all.info "*/usr/*" "*src/tests*" "*brick_game/snake/snake.h*" -o s21_snake.info
	-genhtml --rc branch_coverage=1 -o report s21_snake.info
	-make clean

dvi: readme.tex
	# sudo apt-get install texlive-xetex
	# sudo apt-get install texlive-lang-cyrillic
	# sudo apt-get install cm-super
	latexmk -output-directory=dvi/ readme.tex
	#xelatex -output-directory=dvi/ readme.tex
	#latexmk -output-directory=dvi/ -pdf readme.tex

dist:
	@mkdir -p $(BINDIR) $(DOCDIR)
	@make
	@cp -r $(SOURCES) $(BINDIR)
	@make dvi
	@cp -r $(DOCPAGES)/*pdf $(DOCDIR)
	@tar -czvf $(TARFILE).tar.gz $(BINDIR) $(DOCDIR)

clean:
	rm -rf out/*.* dvi/ docdir/ bindir/ *.tar.gz $(T1) $(T2) $(T3) $(T4) test *.info *.gcda *.gcno

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./snake_cli

clang:
	clang-format -i brick_game/snake/*.cpp brick_game/snake/*.h gui/desktop/*.cpp gui/desktop/*.h brick_game/*.h gui/cli/*.c gui/cli/*.h brick_game/tetris/*.c brick_game/tetris/*.h tests/*.cpp tests/*.h

clang_check:
	clang-format -n brick_game/snake/*.cpp brick_game/snake/*.h gui/desktop/*.cpp gui/desktop/*.h brick_game/*.h gui/cli/*.c gui/cli/*.h brick_game/tetris/*.c brick_game/tetris/*.h tests/*.cpp tests/*.h
.PHONY: clang_check

out/%.o: gui/cli/%.c
	@mkdir -p out
	$(CXX) $(CXXFLAGS) -c $< -o $@

out/%.o: brick_game/snake/%.cpp
	@mkdir -p out
	$(CXX) $(CXXFLAGS) -c $< -o $@

out/tetris.o: brick_game/tetris/tetris.c
	@mkdir -p out
	$(CC) $(CFLAGS) -c $< -o out/tetris.o

out/view_c.o: gui/cli/view.c
	@mkdir -p out
	$(CC) $(CFLAGS) -c $< -o out/view_c.o

ext_flag:
	$(eval CXXFLAGS:=$(GCOV))
.PHONY: ext_flag

run:
	./$(T2)
.PHONY: run

run_snake_cli:
	./$(T1)
.PHONY: run_snake_cli

run_tetris:
	./$(T4)
.PHONY: run_tetris

run_tetris_cli:
	./$(T3)
.PHONY: run_tetris_cli

dep:
	sudo apt install qtcreator
	sudo apt install build-essential
	sudo apt install qtbase5-dev build-essential
	sudo apt install latexmk
.PHONY: dep