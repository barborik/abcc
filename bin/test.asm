	global  main

	section .text
	; line 0
main:
	push	rbp
	mov		rbp, rsp
	; line 0
	; line 0
	; line 0
	; line 5
	; line 0
	sub		rsp, 4
	; line 6
	mov		rax, 12
	; line 6
	; line 0
	mov		[rbp - 4], eax
	; line 7
	; line 0
	mov		rax, printf
	; line 0
	push	rax
	; line 0
	; line 7
	lea		rdi, S2
	xor		rax, rax
	sub		rsp, 36
	call	[rbp - 12]
	add		rsp, 36
	mov		rax, rax
	add		rsp, 8
	; line 8
	mov		rax, 0
	; line 0
	mov		rax, rax
	leave
	ret

	extern printf

	section .bss

	section .data
S2:				db 67, 65, 85, 10, 0
