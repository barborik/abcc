; --------------------------------------
;   Adam Barborík 2021
;   SPŠE Ječná
; --------------------------------------

[BITS 16]
[ORG 0x7c00]

mov [BOOT_DISK], dl     	            ; dl stores the disk number right after boot

xor ax, ax                            	; int 13h needs segment registers to be set to 0, which is not the default on (some?) real hardware
mov ds, ax
mov ss, ax
;mov cs, ax
mov es, ax
mov sp, BOOT_ORIGIN                    	; put stack right under bootloader code, on x86 the stack grows downwards
mov bp, BOOT_ORIGIN

mov ah, 0				                ; set video mode
mov al, 3				                ; 80x25 16 color text (CGA,EGA,MCGA,VGA)
int 0x10

mov ah, 2				                ; read disk sectors
mov al, 128				                ; number of sectors to read (1-128 decimal)
mov ch, 0				                ; track/cylinder number (0-1023 decimal)
mov cl, 2				                ; sector number | 1 = boot sector, 2->n = kernel
mov dh, 0				                ; head number (0-15 decimal)
mov dl, [BOOT_DISK]      	            ; drive number
mov bx, MAIN_ORIGIN                     ; es:bx = pointer to buffer (where to load)
int 0x13

jc display_error                   	    ; display error code on fail

call MAIN_ORIGIN                        ; call the kernel code
jmp $                                	; fail safe

display_error:                        	; subtract 65 ('A') from the ascii value of the character on screen to get the error code
mov bx, VIDEO_MEMORY
mov es, bx
add ah, 'A'
mov [es:0], ah
jmp $

BOOT_DISK:		        db	0
BOOT_ORIGIN:        	equ	0x7c00
MAIN_ORIGIN:        	equ	0x7e00
VIDEO_MEMORY:       	equ	0xb800

times 510 - ( $ - $$ ) db 0		        ; MBR padding
db 0x55, 0xaa				            ; make bootable
