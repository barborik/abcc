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
	; line 16
	; line 802
	sub		rsp, 24
	; line 18
	mov		rax, 9
	; line 18
	; line 0
	mov		rbx, rbp
	sub		rbx, 36
	; line 131599216
	mov		rcx, 4
	; line 18
	mov		rdx, 3
	; line 0
	imul	rcx, rdx
	; line 0
	add		rbx, rcx
	; line 0
	; line 0
	mov		[rbx], eax
	; line 0
	; line 20
	; line 0
	sub		rsp, 8
	; line 20
	; line 0
	mov		rax, rbp
	sub		rax, 36
	; line 20
	mov		rbx, 12
	; line 0
	add		rax, rbx
	; line 0
	inc		QWORD [rax]
	; line 20
	; line 0
	mov		[rbp - 44], rax
	; line 22
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 22
	mov		rdi, S5
	; line 22
	; line 0
	mov		rsi, rbp
	sub		rsi, 44
	; line 0
	mov		rsi, [rsi]
	; line 0
	mov		esi, DWORD [rsi]
	xor		rax, rax
	sub		rsp, 44
	call	[rbp - 52]
	add		rsp, 44
	mov		rax, rax
	add		rsp, 8
	; line 24
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
S5:				db 37, 100, 10, 0
