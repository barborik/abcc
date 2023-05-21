SRC = hello.abc

ABCC = ../bin/abcc
TARGET = test

ifeq ($(OS),Windows_NT)
RM = del
AFLAGS = -f win64
else
RM = rm
AFLAGS = -f elf64
endif

$(TARGET): test.o
	gcc $^ -o $@ -no-pie -lGL -lglfw
	$(RM) $^
	@echo === RUNNING ===
	@./$@ 1


test.o: test.asm
	nasm $(AFLAGS) $^ -o $@
	$(RM) $^

test.asm: $(SRC)
	$(ABCC) $^ $@
