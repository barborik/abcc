CC = gcc

CFLAGS = -O2 -Wall -Wextra -Wpedantic -Wno-unused-function
LFLAGS = 

INCL = 
LIBS = 

TARGET = bin/abcc

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

ifeq ($(OS),Windows_NT)
TARGET += .exe
endif

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LIBS) $(LFLAGS)

obj/%.o: src/%.c
	$(CC) -c $< -o $@ $(INCL) $(CFLAGS)