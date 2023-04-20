abcc.exe test.abc test.asm
nasm -f win64 test.asm -o test.o
gcc test.o -fPIE -no-pie
echo "running a.out"
a.exe
