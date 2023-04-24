SRC = hello.abc

ABCC = ../bin/abcc
TARGET = test

ifeq ($(OS),Windows_NT)
RM = del
else
RM = rm
endif

test.exe: test.o
	gcc $^ -o $@ -fPIE -no-pie
	$(RM) $^
	@echo === RUNNING ===
	@$@


test.o: test.asm
	nasm -f win64 $^ -o $@
	$(RM) $^

test.asm: $(SRC)
	$(ABCC) $^ $@
