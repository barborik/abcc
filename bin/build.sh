./abcc fib.abc test.asm
nasm -f elf64 test.asm

gcc -c print.c
gcc test.o print.o -fPIE -no-pie
echo "running a.out"
./a.out