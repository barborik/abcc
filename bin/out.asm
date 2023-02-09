	global  main
	extern  printf
	extern  exit

	section .bss
abc:		dq 0

	section .text
format:
	db		"%d", 10, 0
printint:
	mov		rsi, rax
	mov		rdi, format
	sub		rsp, 32
	call	printf
	add		rsp, 32
	ret

main:
	mov		rax, 8
	mov		rbx, 2
	mov		rcx, 6
	imul	rbx, rcx
	add		rax, rbx
	mov		[abc], rax
	mov		rax, 2
	mov		[abc], rax
	mov		rax, 4
	mov		[abc], rax
	mov		rax, 8
	mov		[abc], rax
	mov		rax, 10
	mov		[abc], rax
	mov		rax, 12
	mov		[abc], rax
	mov		rax, 14
	mov		[abc], rax

	call printint
	mov		rsi, 0
	sub		rsp, 32
	call	exit
