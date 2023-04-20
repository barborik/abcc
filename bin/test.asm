	global  main

	section .text
	; line 0
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 4
	mov		[rbp - 4], edi
	sub		rsp, 8
	mov		[rbp - 12], rsi
	; line 0
	; line 0
	; line 16
	; line 807
	sub		rsp, 4
	; line 16
	mov		rax, 5
	; line 16
	; line 813
	mov		[rbp - 16], eax
	; line 18
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
