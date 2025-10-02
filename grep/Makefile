CC     = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

SOURCES = grep.c grep_utils.c pattern_utils.c
TARGET  = s21_grep

all: s21_grep
s21_grep: $(TARGET)

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
	leaks -atExit -- ./$(TARGET) -e char --regexp=rag -f tests/test_5.txt -cln tests/test_6.txt tests/test_9.txt
	make clean

valgrind:
	make all
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET) -e char --regexp=rag -f tests/test_5.txt -cln tests/test_6.txt tests/test_9.txt
	make clean

test:
	make $(TARGET)
	bash run_test.sh
	make clean
