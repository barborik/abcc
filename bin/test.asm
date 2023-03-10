	global  main
	extern  printf
	extern  exit


	section .text
format:
	db		"%zu", 10, 0
printint:
	mov		rsi, [abc]
	mov		rdi, format
	sub		rsp, 32
	call	printf
	add		rsp, 32
	ret

main:
	mov		rax, 1
	mov		[abc], rax

	call printint
	mov		rsi, 0
	sub		rsp, 32
	call	exit

	section .bss
abc:		dd 0
