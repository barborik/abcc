	global  main
	extern  printf
	extern  ExitProcess

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
	mov		rax, 2
	mov		rbx, 3
	imul	rax, rbx
	mov		rbx, 1
	add		rax, rbx

	call printint
	mov		rcx, 0
	sub		rsp, 32
	call	ExitProcess
