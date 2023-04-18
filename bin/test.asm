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
	; line 0
	; line 0
	; line 5
	; line 812
	sub		rsp, 24
	; line 7
	mov		rax, 2
	; line 7
	; line 0
	mov		rbx, rbp
	sub		rbx, 36
	; line -1610100496
	mov		rcx, 4
	; line 7
	mov		rdx, 1
	; line 0
	imul	rcx, rdx
	; line 0
	add		rbx, rcx
	; line 0
	; line 0
	mov		[rbx], eax
	; line 8
	mov		rax, 16
	; line 8
	; line 0
	mov		rbx, rbp
	sub		rbx, 36
	; line -1610100496
	mov		rcx, 4
	; line 8
	mov		rdx, 0
	; line 0
	imul	rcx, rdx
	; line 0
	add		rbx, rcx
	; line 0
	; line 0
	mov		[rbx], eax
	; line 10
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 10
	mov		rdi, S4
	; line 10
	; line 0
	mov		rsi, rbp
	sub		rsi, 36
	; line -1610100496
	mov		rdx, 4
	; line 10
	mov		rcx, 0
	; line 0
	imul	rdx, rcx
	; line 0
	add		rsi, rdx
	; line 0
	mov		rsi, [rsi]
	xor		rax, rax
	sub		rsp, 4
	call	[rbp - 44]
	add		rsp, 4
	mov		rax, rax
	add		rsp, 8
	; line 12
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar
	extern atoi

	section .bss

	section .data
S4:				db 37, 100, 10, 0
