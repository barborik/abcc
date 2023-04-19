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
	; line 805
	; line 816
	; line -1122696976
	; line 16
	; line 818
	sub		rsp, 8
	; line 16
	mov		rax, S5
	; line 16
	; line 0
	mov		[rbp - 20], rax
	; line 815
	; line 17
	; line 812
	sub		rsp, 8
	; line 17
	mov		rax, S6
	; line 17
	; line 0
	mov		[rbp - 28], rax
	; line 18
	; line 810
	sub		rsp, 20
	; line 0
	; line 20
	; line 0
	sub		rsp, 4
	; line 20
	mov		rax, 0
	; line 20
	; line 0
	mov		[rbp - 52], eax


	; line 22
	; line 0
	mov		rax, putchar
	; line 0
	push	rax
	; line 0
	; line 22
	; line 0



	mov		rcx, rbp
	sub		rcx, 20
	; line 0
	mov		rcx, [rcx]
	; line 0
	mov		rdx, 1
	; line 22
	; line 0
	mov		r8, rbp
	sub		r8, 52
	mov 	r8d, [r8]
	; line 0
	imul	rdx, r8
	; line 0
	add		rcx, rdx






	; line 0
	mov		ecx, DWORD [rcx]
	xor		rax, rax
	sub		rsp, 36
	call	[rbp - 60]
	add		rsp, 36
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
S5:				db 72, 101, 108, 108, 111, 0
S6:				db 87, 111, 114, 108, 100, 0
