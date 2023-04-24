../../bin/abcc -m16 kernel.abc kernel.asm

rm test.bin

nasm -f bin boot.asm -o boot.bin
nasm -f bin kernel.asm -o kernel.bin

cat boot.bin >> test.bin
cat kernel.bin >> test.bin
cat zeros >> test.bin

qemu-system-x86_64 -net none -usb test.bin
