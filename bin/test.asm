	global  main

	section .text
	; line 0
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 4
	mov		[rbp - 4], ecx
	sub		rsp, 8
	mov		[rbp - 12], rdx
	; line 0
	; line 0
	; line 0
	; line 808
	; line 0
	; line 0
	; line 25
	; line 811
	sub		rsp, 1
	; line 27
	mov		rax, 65
	; line 27
	; line 0
	mov		rbx, rbp
	sub		rbx, 13
	; line 818
	; line 813
	mov		[rbx], al
	; line 29
	; line 0
	mov		rax, putchar
	; line 815
	push	rax
	; line 0
	; line 29
	; line 0
	mov		rcx, rbp
	sub		rcx, 13
	; line 0
	inc		QWORD [rcx]
	; line 0
	mov		cl, BYTE [rcx]
	xor		rax, rax
	sub		rsp, 43
	call	[rbp - 21]
	add		rsp, 43
	mov		rax, rax
	add		rsp, 8
	; line 32
	; line 812
	sub		rsp, 8
	; line 33
	; line 0
	mov		rax, putchar
	; line 0
	; line 33
	; line 0
	mov		rbx, rbp
	sub		rbx, 21
	; line 0
	; line 0
	mov		[rbx], rax
	; line 34
	; line 0
	mov		rax, rbp
	sub		rax, 21
	; line 0
	push	rax
	; line 0
	; line 34
	; line 0
	mov		rcx, rbp
	sub		rcx, 13
	; line 0
	mov		cl, BYTE [rcx]
	xor		rax, rax
	sub		rsp, 35
	call	[rbp - 29]
	add		rsp, 35
	mov		rax, rax
	add		rsp, 8
	; line 36
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar
	extern atoi
	extern strcpy

	section .bss

	section .data
