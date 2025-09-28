CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = calculoMatrizes.c gerenciar-matriz/gerenciar-matriz.c
OBJ = $(patsubst %.c,dist/%.o,$(SRC))
TARGET = dist/main

$(shell mkdir -p dist/gerenciar-matriz)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

dist/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf dist

.PHONY: all clean