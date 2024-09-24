SHELL = /bin/sh
GCC = gcc -Wall -Wextra -Werror -std=c11 -g
FILES = brickgame.c brick_game/tetris/tetris.c gui/cli/frontend.c brick_game/brickgame.c
BACK = brick_game/tetris/tetris.c brick_game/brickgame.c
OS = $(shell uname)

all: clean install 

lib: $(BACK)
	mkdir -p build
	$(GCC) -c $(BACK)
	ar rcs build/libtetris.a *.o
	mv *.o build

install: clean lib
	$(GCC) brickgame.c gui/cli/frontend.c -lncurses -Lbuild -ltetris -o build/tetris

uninstall:
	rm -rf build

dist: install
	tar -czvf tetris.tar.gz Makefile brick_game gui brickgame.c

dvi: clean
	doxygen Doxyfile
	open html/index.html

test:
	

gcov_report:


clean:
	rm -rf build tetris *.o *.a tetris.tar.gz high_score.txt html latex

# style:
# 	cp ../materials/linters/.clang-format .
# 	clang-format -n $(FILES)
# 	rm .clang-format

dox_install:
ifeq ($(OS), Linux)
	sudo apt install -y doxygen
else
	brew install doxygen
endif

clang:
	clang-format -i $(FILES) gui/cli/frontend.h brick_game/brickgame.h brick_game/tetris/backend.h

mem:
ifeq ($(OS),Linux)
	valgrind --tool=memcheck --leak-check=yes ./build/tetris
else
	leaks --atExit -- ./build/tetris
endif

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem $(FILES)

lint:
	s21lint $(FILES)