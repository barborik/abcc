	global  main

	section .text
	; line 958464
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
	; line 0
	; line 5
	; line 0
	sub		rsp, 24
	; line 7
	mov		rax, 2
	; line 7
	; line 0
	mov		rbx, rbp
	sub		rbx, 36
	; line 0
	mov		rcx, 4
	; line 7
	mov		rdx, 1
	; line 0
	imul	rcx, rdx
	; line 0
	add		rbx, rcx
	; line 812
	; line 808
	mov		[rbx], eax
	; line 8
	mov		rax, 16
	; line 8
	; line 810
	mov		rbx, rbp
	sub		rbx, 36
	; line 0
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
	mov		rcx, S4
	; line 10
	; line 0
	mov		rdx, rbp
	sub		rdx, 36
	; line 0
	mov		r8, 4
	; line 10
	mov		r9, 0
	; line 0
	imul	r8, r9
	; line 0
	add		rdx, r8
	; line 0
	mov		rdx, [rdx]
	xor		rax, rax
	sub		rsp, 20
	call	[rbp - 44]
	add		rsp, 20
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
