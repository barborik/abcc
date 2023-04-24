	global  main

	section .text
	; line 0
main:
	push	bp
	mov		bp, sp
	; line 0
	; line 16
	; line 0
	mov		ax, putc
	; line 0
	; line 0
	; line 0
	; line 0
	; line 16
	mov		bx, 76
	push	bx
	; line 16
	mov		bx, 76
	push	bx
	; line 16
	mov		bx, 76
	push	bx
	call	ax
	add		sp, 6
	mov		ax, ax
	; line 0
	leave
	ret

	; line 806
putc:
	push	bp
	mov		bp, sp
	; line 805
	; line 0
	; line 0
	; line 6
	; line 0
	sub		sp, 2
	; line 6
	mov		al, BYTE [bp - -8]
	; line 6
	mov		bx, 80
	; line 0
	; line 6
	mov		bl, BYTE [bp - -6]
	; line 816
	add		ax, bx
	; line 6
	mov		bx, 2
	; line 818
	; line 6
	; line 808
	mov		[bp - 2], ax
	; line 9

        mov ax, 0xB800
        mov es, ax
        mov bx, word 0
        mov al, byte [bp - -4]
        mov [es:bx], al
    	; line 0
	leave
	ret


	section .bss

	section .data
