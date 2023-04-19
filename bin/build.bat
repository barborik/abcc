rem abcc.exe test.abc test.asm
nasm -f elf64 test.asm -o test.o
gcc test.o -fPIE -no-pie
echo "running a.out"
a.exe
