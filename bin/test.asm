	global  main

	section .text
	; line 29
main:
	sub		rsp, 8
	; line 0
	; line 31
	; line 0
	; line 31
	mov		rdi, 10
	sub		rsp, 0
	call	fib_
	add		rsp, 0
	; line 32
	mov		rdi, 0
	; line 32
	mov		rax, rdi
	add		rsp, 8
	ret

	; line 3
fib_:
	sub		rsp, 8
	mov		[rsp - 0], rdi
	; line 0
	; line 0
	; line 0
	; line 0
	; line 0
	; line 6
	mov		rdi, 0
	; line 6
	; line 6
	mov		[rsp - 4], edi
	; line 12
	mov		rdi, 0
	; line 12
	; line 12
	mov		[rsp - 8], edi
	; line 13
	mov		rdi, 1
	; line 13
	; line 13
	mov		[rsp - 12], edi
	; line 14
	mov		rdi, 0
	; line 14
	; line 14
	mov		[rsp - 16], edi
	; line 16
L1:
	; line 16
	mov		edi, [rsp - 4]
	; line 16
	mov		esi, [rsp - 0]
	; line 16
	cmp		rdi, rsi
	setl	dil
	and		rdi, 0xff
	cmp		rdi, 0
	je		L2
	; line 0
	; line 0
	; line 0
	; line 0
	; line 18
	; line 0
	; line 18
	mov		edi, [rsp - 8]
	sub		rsp, 40
	call	print1i
	add		rsp, 40
	; line 20
	mov		edi, [rsp - 8]
	; line 20
	mov		esi, [rsp - 12]
	; line 20
	add		rdi, rsi
	; line 20
	; line 20
	mov		[rsp - 16], edi
	; line 21
	mov		edi, [rsp - 12]
	; line 21
	; line 21
	mov		[rsp - 8], edi
	; line 22
	mov		edi, [rsp - 16]
	; line 22
	; line 22
	mov		[rsp - 12], edi
	; line 24
	mov		edi, [rsp - 4]
	; line 24
	mov		rsi, 1
	; line 24
	add		rdi, rsi
	; line 24
	; line 24
	mov		[rsp - 4], edi
	jmp		L1
L2:
	; line 26
	add		rsp, 8
	ret

	extern print1i

	section .bss
