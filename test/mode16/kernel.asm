	[BITS 16]
	[ORG 0x7e00]

	global  main

	section .text
	; line 0
main:
	push	bp
	mov		bp, sp
	; line 0
	; line 0
	; line 0
	; line 39
	; line 0
	sub		sp, 1
	; line 0
	; line 0
	; line 40
	; line 0
	sub		sp, 1
	; line 40
	mov		ax, 0
	; line 40
	; line 0
	mov		[bp - 2], al
	; line 0
	; line 40
	; line 0
	sub		sp, 1
	; line 40
	mov		ax, 0
	; line 40
	; line 0
	mov		[bp - 3], al
	; line 0
L1:
	; line 42
	mov		ax, 1
	cmp		ax, 0
	je		L2
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 44
	; line 0
	mov		ax, clear
	; line 0
	call	ax
	add		sp, 0
	mov		ax, ax
	; line 45
	; line 0
	mov		ax, putc
	; line 0
	; line 0
	; line 0
	; line 0
	; line 45
	movzx	bx, BYTE [bp - 3]
	push	bx
	; line 45
	movzx	bx, BYTE [bp - 2]
	push	bx
	; line 45
	mov		bx, 64
	push	bx
	call	ax
	add		sp, 6
	mov		ax, ax
	; line 47
	; line 0
	mov		ax, getc
	; line 0
	call	ax
	add		sp, 0
	mov		ax, ax
	; line 47
	; line 0
	mov		[bp - 1], al
	; line 0
	; line 65
	movsx	ax, BYTE [bp - 1]
	; line 65
	mov		bx, 119
	; line 0
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L3
	; line 67
	movzx	ax, BYTE [bp - 3]
	; line 67
	mov		bx, 1
	; line 0
	sub		ax, bx
	; line 67
	; line 0
	mov		[bp - 3], al
L3:
	; line 0
	; line 71
	movsx	ax, BYTE [bp - 1]
	; line 71
	mov		bx, 115
	; line 0
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L4
	; line 73
	movzx	ax, BYTE [bp - 3]
	; line 73
	mov		bx, 1
	; line 0
	add		ax, bx
	; line 73
	; line 0
	mov		[bp - 3], al
L4:
	; line 0
	; line 77
	movsx	ax, BYTE [bp - 1]
	; line 77
	mov		bx, 97
	; line 0
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L5
	; line 79
	movzx	ax, BYTE [bp - 2]
	; line 79
	mov		bx, 1
	; line 0
	sub		ax, bx
	; line 79
	; line 0
	mov		[bp - 2], al
L5:
	; line 0
	; line 83
	movsx	ax, BYTE [bp - 1]
	; line 83
	mov		bx, 100
	; line 0
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L6
	; line 85
	movzx	ax, BYTE [bp - 2]
	; line 85
	mov		bx, 1
	; line 0
	add		ax, bx
	; line 85
	; line 0
	mov		[bp - 2], al
L6:
	jmp		L1
L2:
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
	movzx	ax, BYTE [bp - -8]
	; line 8
	mov		bx, 80
	; line 0
	imul	ax, bx
	; line 8
	movzx	bx, BYTE [bp - -6]
	; line 0
	add		ax, bx
	; line 8
	mov		bx, 2
	; line 0
	imul	ax, bx
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

	; line 0
getc:
	push	bp
	mov		bp, sp
	; line 0
	; line 0
	; line 18
	; line 0
	sub		sp, 1
	; line 20

        mov ah, 0
        int 0x16
        mov [bp - 1], al
    	; line 21
	movsx	ax, BYTE [bp - 1]
	; line 0
	mov		ax, ax
	leave
	ret

	; line 0
clear:
	push	bp
	mov		bp, sp
	; line 0
	; line 0
	; line 0
	; line 0
	; line 26
	; line 0
	sub		sp, 2
	; line 26
	mov		ax, 80
	; line 26
	mov		bx, 25
	; line 0
	imul	ax, bx
	; line 26
	; line 0
	mov		[bp - 2], ax
	; line 27
	mov		ax, WORD [bp - 2]
	; line 27
	mov		bx, 2
	; line 0
	imul	ax, bx
	; line 27
	; line 0
	mov		[bp - 2], ax
	; line 0
	; line 0
	; line 29
	; line 0
	sub		sp, 2
	; line 29
	mov		ax, 0
	; line 29
	; line 0
	mov		[bp - 4], ax
	jmp		L9
L7:
	; line 29
	mov		ax, WORD [bp - 4]
	; line 29
	mov		bx, 2
	; line 0
	add		ax, bx
	; line 29
	; line 0
	mov		[bp - 4], ax
L9:
	; line 29
	mov		ax, WORD [bp - 4]
	; line 29
	mov		bx, WORD [bp - 2]
	; line 0
	cmp		ax, bx
	setl	al
	and		ax, 0xff
	cmp		ax, 0
	je		L8
	; line 32

            mov ax, 0xB800
            mov es, ax
            mov bx, WORD [bp - 4]
            mov [es:bx], BYTE 0
        	jmp		L7
L8:
	; line 0
	leave
	ret


	section .bss

	section .data
