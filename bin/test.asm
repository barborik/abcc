	global  main

	section .text
	; line 7
main:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 4
	mov		[rbp - 4], edi
	sub		rsp, 8
	mov		[rbp - 12], rsi
	; line 0
	; line 9
	; line 0
	; line 0
	; line 9
	mov		rdi, S6
	; line 9
	mov		rsi, QWORD [rbp - 12]
	; line 9
	mov		rdx, 1
	; line 0
	mov		rcx, 8
	; line 0
	imul	rdx, rcx
	; line 9
	add		rsi, rdx
	; line 9
	mov		rsi, [rsi]
	xor		rax, rax
	sub		rsp, 0
	call	printf
	add		rsp, 0
	; line 10
	mov		rax, 0
	; line 10
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar
	extern malloc
	extern fflush
	extern stdout

	section .bss

	section .data
S6:				db 37, 115, 10, 0
