	global  main
	extern  printf
	extern  ExitProcess

	section .bss
abc:		dd 0

	section .text
format:
	db		"%d", 10, 0
printint:
	mov		rdx, rax
	mov		rcx, format
	sub		rsp, 32
	call	printf
	add		rsp, 32
	ret

main:
	mov		rax, 4
	mov		rbx, 2
	mov		rcx, 6
	imul	rbx, rcx
	add		rax, rbx
	mov		abc, rax

	call printint
	mov		rcx, 0
	sub		rsp, 32
	call	ExitProcess
