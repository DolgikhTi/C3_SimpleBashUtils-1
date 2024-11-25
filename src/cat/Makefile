CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TARGET = s21_cat

all: $(TARGET)

$(TARGET): s21_cat.c
	$(CC) $(CFLAGS) $< -o $@

style:
	clang-format -style=google -n s21_cat.c

test:
	./test_cat.sh

clean:
	rm -rf $(TARGET) *.txt