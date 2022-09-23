CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
DBGFLAGS = -g

SRC = $(wildcar *.c)
TARGET = ifj22

default: compile

compile:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

debug:
	$(CC) $(CFLAGS) $(DBGFLAGS) $(SRC) -o $(TARGET)

run: compile
	./$(NAME)
