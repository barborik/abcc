	[BITS 16]
	[ORG 0x7e00]

	global  main

	section .text
	; line 0
main:
	push	bp
	mov		bp, sp
	; line 0
	; line 18
	; line 0
	mov		ax, putc
	; line 0
	; line 0
	; line 0
	; line 0
	; line 18
	mov		bx, 1
	push	bx
	; line 18
	mov		bx, 1
	push	bx
	; line 18
	mov		bx, 76
	push	bx
	call	ax
	add		sp, 6
	mov		ax, ax
	; line 0
	leave
	ret

	; line 0
putc:
	push	bp
	mov		bp, sp
	; line 0
	; line 0
	; line 0
	; line 8
	; line 0
	sub		sp, 2
	; line 8
	mov		al, BYTE [bp - -8]
	; line 8
	mov		bx, 80
	; line 0
	; line 8
	mov		bl, BYTE [bp - -6]
	; line 0
	add		ax, bx
	; line 8
	mov		bx, 2
	; line 0
	; line 8
	; line 0
	mov		[bp - 2], ax
	; line 11

        mov ax, 0xB800
        mov es, ax
        mov bx, word [bp - 2]
        mov al, byte [bp - -4]
        mov [es:bx], al
    	; line 0
	leave
	ret


	section .bss

	section .data
