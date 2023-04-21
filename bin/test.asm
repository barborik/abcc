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
	; line 813
	; line 0
	; line 16
	; line 0
	sub		rsp, 4
	; line 16
	mov		rax, 4
	; line 16
	mov		rbx, 2
	; line 0
	xor		rcx, rcx
	test	rax, rax
	setz	cl
	mov		rax, rcx
	xor		rcx, rcx
	test	rax, rax
	setz	cl
	mov		rax, rcx
	xor		rcx, rcx
	test	rbx, rbx
	setz	cl
	mov		rbx, rcx
	xor		rcx, rcx
	test	rbx, rbx
	setz	cl
	mov		rbx, rcx
	or		rax, rbx
	; line 16
	; line 809
	mov		[rbp - 16], eax
	; line 18
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 18
	mov		rcx, S5
	; line 18
	mov		edx, DWORD [rbp - 16]
	xor		rax, rax
	sub		rsp, 40
	call	[rbp - 24]
	add		rsp, 40
	mov		rax, rax
	add		rsp, 8
	; line 23
	mov		rax, 0
	; line 811
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar
	extern atoi
	extern strcpy

	section .bss

	section .data
S5:				db 37, 100, 10, 0
