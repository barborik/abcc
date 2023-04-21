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
	; line 0
	; line 0
	; line 0
	; line 806
	; line 0
	; line 807
	; line 0
	; line 5
	; line 817
	sub		rsp, 8
	; line 5
	mov		rax, S5
	; line 5
	; line 818
	mov		[rbp - 20], rax
	; line 804
	; line 6
	; line 0
	sub		rsp, 8
	; line 6
	mov		rax, S6
	; line 6
	; line 812
	mov		[rbp - 28], rax
	; line 7
	; line 819
	sub		rsp, 20
	; line 814
	; line 9
	; line 0
	sub		rsp, 4
	; line 9
	; line 820
	sub		rsp, 4
	; line 0
	; line 11
	mov		rax, 0
	; line 11
	; line 0
	mov		[rbp - 52], eax
	jmp		L3
L1:
	; line 11
	mov		eax, DWORD [rbp - 52]
	; line 11
	; line 0
	mov		rbx, rbp
	sub		rbx, 52
	; line 0
	inc		QWORD [rbx]
	; line 0
L3:
	; line 11
	mov		rax, QWORD [rbp - 20]
	; line 0
	mov		rbx, 1
	; line 11
	mov		ecx, DWORD [rbp - 52]
	; line 0
	imul	rbx, rcx
	; line 0
	add		rax, rbx
	; line 0
	mov		rax, [rax]
	; line 11
	mov		rbx, 0
	; line 0
	cmp		al, bl
	setne	al
	and		rax, 0xff
	cmp		rax, 0
	je		L2
	; line 13
	mov		rax, QWORD [rbp - 20]
	; line 0
	mov		rbx, 1
	; line 13
	mov		ecx, DWORD [rbp - 52]
	; line 0
	imul	rbx, rcx
	; line 0
	add		rax, rbx
	; line 0
	mov		rax, [rax]
	; line 13
	; line 0
	mov		rbx, rbp
	sub		rbx, 48
	; line 0
	mov		rcx, 1
	; line 13
	mov		edx, DWORD [rbp - 52]
	; line 0
	imul	rcx, rdx
	; line 0
	add		rbx, rcx
	; line 0
	; line 0
	mov		[rbx], eax
	jmp		L1
L2:
	; line 0
	; line 16
	mov		rax, 0
	; line 16
	; line 0
	mov		[rbp - 56], eax
	jmp		L6
L4:
	; line 16
	mov		eax, DWORD [rbp - 56]
	; line 16
	; line 0
	mov		rbx, rbp
	sub		rbx, 56
	; line 0
	inc		QWORD [rbx]
	; line 0
L6:
	; line 16
	mov		rax, QWORD [rbp - 28]
	; line 0
	mov		rbx, 1
	; line 16
	mov		ecx, DWORD [rbp - 56]
	; line 0
	imul	rbx, rcx
	; line 0
	add		rax, rbx
	; line 0
	mov		rax, [rax]
	; line 16
	mov		rbx, 0
	; line 0
	cmp		al, bl
	setne	al
	and		rax, 0xff
	cmp		rax, 0
	je		L5
	; line 18
	mov		rax, QWORD [rbp - 28]
	; line 0
	mov		rbx, 1
	; line 18
	mov		ecx, DWORD [rbp - 56]
	; line 0
	imul	rbx, rcx
	; line 0
	add		rax, rbx
	; line 0
	mov		rax, [rax]
	; line 18
	; line 0
	mov		rbx, rbp
	sub		rbx, 48
	; line 0
	mov		rcx, 1
	; line 18
	mov		edx, DWORD [rbp - 52]
	; line 18
	mov		esi, DWORD [rbp - 56]
	; line 0
	add		rdx, rsi
	; line 0
	imul	rcx, rdx
	; line 0
	add		rbx, rcx
	; line 0
	; line 0
	mov		[rbx], eax
	jmp		L4
L5:
	; line 21
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 21
	mov		rcx, S7
	; line 21
	; line 0
	mov		rdx, rbp
	sub		rdx, 48
	xor		rax, rax
	sub		rsp, 64
	call	[rbp - 64]
	add		rsp, 64
	mov		rax, rax
	add		rsp, 8
	; line 23
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
S5:				db 72, 101, 108, 108, 111, 0
S6:				db 87, 111, 114, 108, 100, 0
S7:				db 37, 115, 10, 0
