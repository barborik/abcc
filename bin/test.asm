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
	; line 5
	; line 813
	sub		rsp, 4
	; line 5
	mov		rax, -1
	; line 5
	; line 804
	mov		[rbp - 16], eax
	; line 7
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 0
	; line 7
	mov		rdi, S4
	; line 7
	; line 0
	mov		rsi, rbp
	sub		rsi, 16
	; line 0
	mov		esi, DWORD [rsi]
	xor		rax, rax
	sub		rsp, 8
	call	[rbp - 24]
	add		rsp, 8
	mov		rax, rax
	add		rsp, 8
	; line 9
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf
	extern putchar
	extern atoi

	section .bss

	section .data
S4:				db 37, 100, 10, 0
