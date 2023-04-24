..\..\bin\abcc.exe -m16 kernel.abc kernel.asm

del test.asm

type boot.asm >> test.asm
type kernel.asm >> test.asm

nasm -f bin test.asm -o test.bin

type zeros >> test.bin
