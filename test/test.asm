	global  main

	section .text
	; line 0
main:
	push	rbp
	mov		rbp, rsp
	; line 0
	; line 28
	; line 0
	mov		rax, fib
	; line 0
	push	rax
	; line 0
	; line 28
	mov		rdi, 10
	xor		rax, rax
	sub		rsp, 40
	call	[rbp - 8]
	add		rsp, 40
	mov		rax, rax
	add		rsp, 8
	; line 29
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	; line 0
fib:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 4
	mov		[rbp - 4], edi
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 5
	; line 0
	sub		rsp, 4
	; line 5
	mov		rax, 0
	; line 5
	; line 0
	mov		[rbp - 8], eax
	; line 0
	; line 6
	; line 0
	sub		rsp, 4
	; line 6
	mov		rax, 1
	; line 6
	; line 0
	mov		[rbp - 12], eax
	; line 0
	; line 7
	; line 0
	sub		rsp, 4
	; line 7
	mov		rax, 0
	; line 7
	; line 0
	mov		[rbp - 16], eax
	; line 0
	; line 0
	; line 9
	; line 0
	sub		rsp, 4
	; line 9
	mov		rax, 0
	; line 9
	; line 0
	mov		[rbp - 20], eax
	jmp		L3
L1:
	; line 9
	mov		eax, DWORD [rbp - 20]
	; line 9
	; line 0
	mov		rbx, rbp
	sub		rbx, 20
	; line 0
	inc		QWORD [rbx]
	; line 0
L3:
	; line 9
	mov		eax, DWORD [rbp - 20]
	; line 9
	mov		ebx, DWORD [rbp - 4]
	; line 0
	cmp		eax, ebx
	setl	al
	and		rax, 0xff
	cmp		rax, 0
	je		L2
	; line 0
	; line 0
	; line 0
	; line 11
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 11
	lea		rdi, S13
	; line 11
	mov		esi, DWORD [rbp - 8]
	xor		rax, rax
	sub		rsp, 36
	call	[rbp - 28]
	add		rsp, 36
	mov		rax, rax
	add		rsp, 8
	; line 13
	mov		eax, DWORD [rbp - 8]
	; line 13
	mov		ebx, DWORD [rbp - 12]
	; line 0
	add		rax, rbx
	; line 13
	; line 0
	mov		[rbp - 16], eax
	; line 14
	mov		eax, DWORD [rbp - 12]
	; line 14
	; line 0
	mov		[rbp - 8], eax
	; line 15
	mov		eax, DWORD [rbp - 16]
	; line 15
	; line 0
	mov		[rbp - 12], eax
	jmp		L1
L2:
	; line 0
	leave
	ret

	; line 0
2fart:
	push	rbp
	mov		rbp, rsp
	; line 23
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern fopen
	extern fgetc
	extern fseek
	extern fflush
	extern printf
	extern putchar
	extern atoi
	extern strcpy
	extern getchar

	section .bss

	section .data
S13:				db 37, 100, 10, 0
