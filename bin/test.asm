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
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 9
	sub		rsp, 4
	; line 10
	sub		rsp, 8
	; line 11
	; line 0
	; line 11
	mov		rdi, 4
	; line 11
	mov		rsi, 5
	; line 11
	imul	rdi, rsi
	sub		rsp, 0
	call	malloc
	add		rsp, 0
	mov		rax, rax
	; line 11
	; line 11
	mov		[rbp - 24], rax
	; line 13
	mov		rax, 6
	; line 13
	mov		rbx, QWORD [rbp - 24]
	; line 13
	mov		rcx, 0
	; line 0
	mov		rdx, 8
	; line 0
	imul	rcx, rdx
	; line 13
	add		rbx, rcx
	; line 13
	; line 13
	mov		[rbx], rax
	; line 14
	mov		rax, 8
	; line 14
	mov		rbx, QWORD [rbp - 24]
	; line 14
	mov		rcx, 1
	; line 0
	mov		rdx, 8
	; line 0
	imul	rcx, rdx
	; line 14
	add		rbx, rcx
	; line 14
	; line 14
	mov		[rbx], rax
	; line 15
	mov		rax, 7
	; line 15
	mov		rbx, QWORD [rbp - 24]
	; line 15
	mov		rcx, 2
	; line 0
	mov		rdx, 8
	; line 0
	imul	rcx, rdx
	; line 15
	add		rbx, rcx
	; line 15
	; line 15
	mov		[rbx], rax
	; line 17
	; line 0
	; line 0
	; line 17
	mov		rdi, S6
	; line 17
	mov		rsi, QWORD [rbp - 24]
	xor		rax, rax
	sub		rsp, 0
	call	printf
	add		rsp, 0
	; line 18
	mov		rax, 0
	; line 18
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
S6:				db 65, 65, 65, 32, 37, 100, 10, 0
