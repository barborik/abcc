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
	mov		rax, 1
	mov		[abc], rax
L1:
	mov		rax, 1
	cmp		rax, 0
	je		L2
	jmp		L1
L2:

	call printint
	mov		rsi, 0
	sub		rsp, 32
	call	exit
