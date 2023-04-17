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
	; line 14
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 14
	mov		rdi, S5
	; line 14
	; line 0
	mov		rsi, fac
	; line 0
	push	rsi
	push	rdi
	; line 0
	; line 14
	; line 0
	mov		rdi, atoi
	; line 0
	push	rdi
	; line 0
	; line 14
	; line 0
	mov		rdi, rbp
	sub		rdi, 12
	; line 0
	mov		rdi, [rdi]
	; line 282656272
	mov		rsi, 8
	; line 14
	mov		rdx, 1
	; line 0
	imul	rsi, rdx
	; line 0
	add		rdi, rsi
	; line 0
	mov		rdi, [rdi]
	xor		rax, rax
	sub		rsp, 4
	call	[rbp - 44]
	add		rsp, 4
	mov		rdi, rax
	add		rsp, 8
	xor		rax, rax
	sub		rsp, 12
	call	[rbp - 28]
	add		rsp, 12
	mov		rsi, rax
	pop		rdi
	add		rsp, 8
	xor		rax, rax
	sub		rsp, 12
	call	[rbp - 20]
	add		rsp, 12
	mov		rax, rax
	add		rsp, 8
	; line 15
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	; line 0
fac:
	push	rbp
	mov		rbp, rsp
	sub		rsp, 4
	mov		[rbp - 4], edi
	; line 0
	; line 0
	; line 5
	; line 806
	mov		rax, rbp
	sub		rax, 4
	; line 815
	mov		eax, DWORD [rax]
	; line 5
	mov		rbx, 0
	; line 0
	cmp		rax, rbx
	sete	al
	and		rax, 0xff
	cmp		rax, 0
	je		L1
	; line 7
	mov		rax, 1
	; line 0
	mov		rax, rax
	leave
	ret

L1:
	; line 9
	; line 0
	mov		rax, rbp
	sub		rax, 4
	; line 0
	mov		eax, DWORD [rax]
	; line 9
	; line 0
	mov		rbx, fac
	; line 0
	push	rbx
	push	rax
	; line 0
	; line 9
	; line 0
	mov		rdi, rbp
	sub		rdi, 4
	; line 0
	mov		edi, DWORD [rdi]
	; line 9
	mov		rsi, 1
	; line 0
	sub		rdi, rsi
	xor		rax, rax
	sub		rsp, 12
	call	[rbp - 12]
	add		rsp, 12
	mov		rbx, rax
	pop		rax
	add		rsp, 8
	; line 0
	imul	rax, rbx
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar
	extern atoi

	section .bss

	section .data
S5:				db 37, 100, 10, 0
