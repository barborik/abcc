	global  main

	section .text
	; line 0
main:
	push	rbp
	mov		rbp, rsp
	; line 0
	; line 0
	; line 0
	; line 6
	; line 0
	sub		rsp, 4
	; line 6
	mov		rax, 0
	; line 6
	; line 0
	mov		[rbp - 4], eax
L1:
	; line 6
	; line 0
	mov		rax, rbp
	sub		rax, 4
	; line 0
	mov		eax, DWORD [rax]
	; line 6
	mov		rbx, 10
	; line 0
	cmp		rax, rbx
	setl	al
	and		rax, 0xff
	cmp		rax, 0
	je		L2
	; line 8
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 8
	mov		rdi, S3
	; line 8
	; line 0
	mov		rsi, rbp
	sub		rsi, 4
	; line 0
	mov		esi, DWORD [rsi]
	xor		rax, rax
	sub		rsp, 12
	call	[rbp - 12]
	add		rsp, 12
	mov		rax, rax
	add		rsp, 8
	; line 6
	; line 0
	mov		rbx, rbp
	sub		rbx, 4
	; line 0
	mov		ebx, DWORD [rbx]
	; line 6
	; line 0
	mov		rcx, rbp
	sub		rcx, 4
	; line 0
	inc		QWORD [rcx]
	; line 0
	jmp		L1
L2:
	; line 11
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar

	section .bss

	section .data
S3:				db 72, 101, 108, 108, 111, 44, 32, 87, 111, 114, 108, 100, 33, 32, 37, 100, 10, 0
