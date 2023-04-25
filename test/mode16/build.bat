..\..\bin\abcc -m16 kernel.abc kernel.asm

del test.bin

nasm -f bin boot.asm -o boot.bin
nasm -f bin kernel.asm -o kernel.bin

type boot.bin >> test.bin
type kernel.bin >> test.bin
type zeros >> test.bin

qemu-system-x86_64 -net none -usb test.bin
