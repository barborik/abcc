SRC = main.abc

ABCC = ../bin/abcc
TARGET = main

ifeq ($(OS),Windows_NT)
RM = del
AFLAGS = -f win64
else
RM = rm
AFLAGS = -f elf64
endif

$(TARGET): main.o
	gcc $^ -o $@ -no-pie -lGL -lglfw
	$(RM) $^
	@echo === RUNNING ===
	@./$@ 1


main.o: main.asm
	nasm $(AFLAGS) $^ -o $@
	$(RM) $^

main.asm: $(SRC)
	$(ABCC) $^ $@
