CC = gcc

CFLAGS = -g -Wall -Wextra -Wpedantic
LFLAGS = 

INCL = -I lib/tpp/
LIBS = 

TARGET = bin/abcc

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC)) obj/frontend.o obj/tpp.o

ifeq ($(OS),Windows_NT)
TARGET += .exe
endif

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LIBS) $(LFLAGS)

obj/%.o: src/%.c
	$(CC) -c $< -o $@ $(INCL) $(CFLAGS)

obj/%.o: lib/tpp/%.c
	$(CC) -c $< -o $@ $(INCL)
