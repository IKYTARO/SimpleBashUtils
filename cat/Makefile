CC     = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

SOURCES = cat_utils.c cat.c
TARGET  = s21_cat

all: s21_cat 
s21_cat: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(TARGET)

rebuild:
	make clean 
	make all

format:
	clang-format -i *.c *.h 

check-format:
	clang-format -n *.c *.h 

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

leaks:
	make all
	leaks -atExit -- ./$(TARGET) -benstv tests/test_1.txt
	make clean

valgrind:
	make all
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) -benstv tests/test_1.txt
	make clean

test:
	make $(TARGET)
	bash run_test.sh
	make clean
