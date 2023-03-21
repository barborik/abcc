	global  main

	section .text
	; line 35
main:
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 37
	mov		rdi, 37
	; line 37
	; line 0
	mov		rsi, str_
	; line 37
	mov		rdx, 0
	; line 0
	mov		rcx, 1
	; line 0
	imul	rdx, rcx
	; line 37
	add		rsi, rdx
	; line 37
	; line 37
	mov		[rsi], dil
	; line 38
	mov		rdi, 100
	; line 38
	; line 0
	mov		rsi, str_
	; line 38
	mov		rdx, 1
	; line 0
	mov		rcx, 1
	; line 0
	imul	rdx, rcx
	; line 38
	add		rsi, rdx
	; line 38
	; line 38
	mov		[rsi], dil
	; line 39
	mov		rdi, 10
	; line 39
	; line 0
	mov		rsi, str_
	; line 39
	mov		rdx, 2
	; line 0
	mov		rcx, 1
	; line 0
	imul	rdx, rcx
	; line 39
	add		rsi, rdx
	; line 39
	; line 39
	mov		[rsi], dil
	; line 40
	mov		rdi, 0
	; line 40
	; line 0
	mov		rsi, str_
	; line 40
	mov		rdx, 3
	; line 0
	mov		rcx, 1
	; line 0
	imul	rdx, rcx
	; line 40
	add		rsi, rdx
	; line 40
	; line 40
	mov		[rsi], dil
	; line 42
	; line 0
	; line 42
	mov		rdi, 10
	sub		rsp, 8
	call	fib_
	add		rsp, 8
	; line 43
	mov		rdi, 0
	; line 43
	mov		rax, rdi
	ret

	; line 6
fib_:
	sub		rsp, 4
	mov		[rsp + 0], edi
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 8
	sub		rsp, 8
	; line 9
	mov		dil, [str_]
	; line 9
	mov		rsi, str_
	; line 9
	; line 9
	mov		[rsp + 0], rsi
	; line 11
	sub		rsp, 4
	; line 12
	mov		rdi, 0
	; line 12
	; line 12
	mov		[rsp + 0], edi
	; line 14
	sub		rsp, 4
	; line 15
	sub		rsp, 4
	; line 16
	sub		rsp, 4
	; line 18
	mov		rdi, 0
	; line 18
	; line 18
	mov		[rsp + 8], edi
	; line 19
	mov		rdi, 1
	; line 19
	; line 19
	mov		[rsp + 4], edi
	; line 20
	mov		rdi, 0
	; line 20
	; line 20
	mov		[rsp + 0], edi
	; line 22
L1:
	; line 22
	mov		edi, [rsp + 12]
	; line 22
	mov		esi, [rsp + 24]
	; line 22
	cmp		rdi, rsi
	setl	dil
	and		rdi, 0xff
	cmp		rdi, 0
	je		L2
	; line 0
	; line 0
	; line 0
	; line 0
	; line 24
	; line 0
	; line 0
	; line 24
	mov		rdi, [rsp + 16]
	; line 24
	mov		esi, [rsp + 8]
	sub		rsp, 12
	call	printf
	add		rsp, 12
	; line 26
	mov		edi, [rsp + 8]
	; line 26
	mov		esi, [rsp + 4]
	; line 26
	add		rdi, rsi
	; line 26
	; line 26
	mov		[rsp + 0], edi
	; line 27
	mov		edi, [rsp + 4]
	; line 27
	; line 27
	mov		[rsp + 8], edi
	; line 28
	mov		edi, [rsp + 0]
	; line 28
	; line 28
	mov		[rsp + 4], edi
	; line 30
	mov		edi, [rsp + 12]
	; line 30
	mov		rsi, 1
	; line 30
	add		rdi, rsi
	; line 30
	; line 30
	mov		[rsp + 12], edi
	jmp		L1
L2:
	; line 32
	add		rsp, 28
	ret

	extern printf
	extern putchar

	section .bss
str_:				resb 4
