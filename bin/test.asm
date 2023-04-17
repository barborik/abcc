	global  main

	section .text
	; line 0
main:
	push	rbp
	mov		rbp, rsp
	; line 0
	; line 5
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 5
	mov		rcx, S3
	xor		rax, rax
	sub		rsp, 24
	call	[rbp - 8]
	add		rsp, 24
	mov		rax, rax
	add		rsp, 8
	; line 7
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar

	section .bss

	section .data
S3:				db 72, 101, 108, 108, 111, 44, 32, 87, 111, 114, 108, 100, 33, 10, 0
