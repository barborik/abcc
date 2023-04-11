	global  main

	section .text
	; line 0
main:
	push	rbp
	mov		rbp, rsp
	; line 0
	; line 0
	; line 0
	; line 0
	; line 6
	sub		rsp, 4
	; line 8
	mov		rax, 0
	; line 8
	; line 0
	mov		[rbp - 4], eax
	; line 10
	mov		rax, QWORD printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 10
	mov		rdi, S3
	; line 10
	; line 0
	mov		rsi, rbp
	sub		rsi, 4
	; line 0
	inc		QWORD [rsi]
	mov		esi, DWORD [rsi]
	; line 0
	mov		rdx, rbp
	sub		rdx, 4
	; line 0
	inc		QWORD [rdx]
	mov		edx, DWORD [rdx]
	xor		rax, rax
	sub		rsp, 12
	call	[rbp - 12]
	add		rsp, 12
	mov		rax, rax
	add		rsp, 8
	; line 12
	mov		rax, QWORD printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 12
	mov		rdi, S3
	; line 12
	mov		esi, DWORD [rbp - 4]
	; line 0
	mov		rdx, rbp
	sub		rdx, 4
	; line 0
	inc		QWORD [rdx]
	mov		edx, DWORD [rdx]
	xor		rax, rax
	sub		rsp, 12
	call	[rbp - 12]
	add		rsp, 12
	mov		rax, rax
	add		rsp, 8
	; line 14
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar

	section .bss

	section .data
S3:				db 37, 100, 10, 0
