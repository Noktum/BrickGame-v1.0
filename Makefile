GCC = gcc -Wall -Wextra -Werror -std=c11
FILES = brickgame.c brick_game/tetris/tetris.c gui/cli/frontend.c brick_game/brickgame.c
BACK = brick_game/tetris/tetris.c brick_game/brickgame.c
OS = $(uname)

all: clean install 

build/libtetris.a: $(BACK)
	mkdir -p build
	$(GCC) -c $(BACK)
	ar rcs build/libtetris.a *.o
	mv *.o build

install: clean build/libtetris.a
	$(GCC) brickgame.c gui/cli/frontend.c -lncurses -Lbuild -ltetris -o build/tetris

uninstall: clean

dist: install
	tar -czvf tetris.tar.gz Makefile brick_game gui brickgame.c
	
clean:
	rm -rf build tetris *.o *.a build
	
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