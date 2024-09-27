SHELL = /bin/sh
GCC = gcc -Wall -Wextra -Werror -std=c11 -g
TEST = -lcheck -lsubunit -lm
FILES = brickgame.c brick_game/tetris/tetris.c gui/cli/frontend.c brick_game/brickgame.c
BACK = brick_game/tetris/tetris.c brick_game/brickgame.c
OS = $(shell uname)

all: clean install dvi
	./build/tetris

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

test: clean lib
	checkmk tests/tetris_test.check > tests/tetris_test.c
	$(GCC) tests/tetris_test.c -Lbuild -ltetris $(TEST) -o test
	clear
	./test

gcov_report: clean
	${GCC} --coverage tests/tetris_test.c $(BACK) $(TEST) -o test
	./test
	lcov --capture --directory . -o coverage.info
	lcov --extract coverage.info '*/brick_game/*' -o coverage_filter.info
	genhtml --output-directory report --legend coverage_filter.info
	open report/index.html
	rm *.gcda *.gcno *.info

clean:
	rm -rf build tetris *.o *.a tetris.tar.gz high_score.txt html latex report *.gcda *.gcno test

# style:
# 	cp ../materials/linters/.clang-format .
# 	clang-format -n $(FILES)
# 	rm .clang-format

dox_install:
ifeq ($(OS), Linux)
	sudo apt install -y doxygen graphviz
else
	brew install doxygen graphviz
endif

clang:
	clang-format -i $(FILES) gui/cli/frontend.h brick_game/brickgame.h brick_game/tetris/backend.h

mem: test
ifeq ($(OS),Linux)
	valgrind -s --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all ./test
else
	leaks --atExit -- ./build/tetris
endif

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem $(FILES)

lint:
	s21lint $(FILES)