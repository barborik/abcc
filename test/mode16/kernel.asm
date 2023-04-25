	[BITS 16]
	[ORG 0x7e00]

	global  main

	section .text
main:
	push	bp
	mov		bp, sp
	sub		sp, 1
	sub		sp, 1
	mov		ax, 0
	mov		[bp - 2], al
	sub		sp, 1
	mov		ax, 0
	mov		[bp - 3], al
L1:
	mov		ax, 1
	cmp		ax, 0
	je		L2
	mov		ax, clear
	call	ax
	add		sp, 0
	mov		ax, ax
	mov		ax, putc
	movzx	bx, BYTE [bp - 3]
	push	bx
	movzx	bx, BYTE [bp - 2]
	push	bx
	mov		bx, 64
	push	bx
	call	ax
	add		sp, 6
	mov		ax, ax
	mov		ax, getc
	call	ax
	add		sp, 0
	mov		ax, ax
	mov		[bp - 1], al
	movsx	ax, BYTE [bp - 1]
	mov		bx, 119
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L3
	movzx	ax, BYTE [bp - 3]
	mov		bx, 1
	sub		ax, bx
	mov		[bp - 3], al
L3:
	movsx	ax, BYTE [bp - 1]
	mov		bx, 115
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L4
	movzx	ax, BYTE [bp - 3]
	mov		bx, 1
	add		ax, bx
	mov		[bp - 3], al
L4:
	movsx	ax, BYTE [bp - 1]
	mov		bx, 97
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L5
	movzx	ax, BYTE [bp - 2]
	mov		bx, 1
	sub		ax, bx
	mov		[bp - 2], al
L5:
	movsx	ax, BYTE [bp - 1]
	mov		bx, 100
	cmp		al, bl
	sete	al
	and		ax, 0xff
	cmp		ax, 0
	je		L6
	movzx	ax, BYTE [bp - 2]
	mov		bx, 1
	add		ax, bx
	mov		[bp - 2], al
L6:
	jmp		L1
L2:
	leave
	ret

putc:
	push	bp
	mov		bp, sp
	sub		sp, 2
	movzx	ax, BYTE [bp - -8]
	mov		bx, 80
	imul	ax, bx
	movzx	bx, BYTE [bp - -6]
	add		ax, bx
	mov		bx, 2
	imul	ax, bx
	mov		[bp - 2], ax

        mov ax, 0xB800
        mov es, ax
        mov bx, word [bp - 2]
        mov al, byte [bp - -4]
        mov [es:bx], al
    	leave
	ret

getc:
	push	bp
	mov		bp, sp
	sub		sp, 1

        mov ah, 0
        int 0x16
        mov [bp - 1], al
    	movsx	ax, BYTE [bp - 1]
	mov		ax, ax
	leave
	ret

clear:
	push	bp
	mov		bp, sp
	sub		sp, 2
	mov		ax, 80
	mov		bx, 25
	imul	ax, bx
	mov		[bp - 2], ax
	mov		ax, WORD [bp - 2]
	mov		bx, 2
	imul	ax, bx
	mov		[bp - 2], ax
	sub		sp, 2
	mov		ax, 0
	mov		[bp - 4], ax
	jmp		L9
L7:
	mov		ax, WORD [bp - 4]
	mov		bx, 2
	add		ax, bx
	mov		[bp - 4], ax
L9:
	mov		ax, WORD [bp - 4]
	mov		bx, WORD [bp - 2]
	cmp		ax, bx
	setl	al
	and		ax, 0xff
	cmp		ax, 0
	je		L8

            mov ax, 0xB800
            mov es, ax
            mov bx, WORD [bp - 4]
            mov [es:bx], BYTE 0
        	jmp		L7
L8:
	leave
	ret


	section .bss

	section .data
