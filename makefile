CC = gcc
CFLAGS = -Wall -Wextra -g -I.
SRC = main.c gerenciar-matriz/gerenciar-matriz.c multiplicao-matriz/multiplicao-matriz.c
OBJ = $(patsubst %.c,dist/%.o,$(SRC))
TARGET = dist/main

$(shell mkdir -p dist/gerenciar-matriz dist/multiplicao-matriz)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm

dist/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf dist

.PHONY: all clean