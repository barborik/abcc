./abcc fib.abc test.asm
nasm -f elf64 test.asm
gcc test.o -fPIE -no-pie
echo "running a.out"
./a.out
