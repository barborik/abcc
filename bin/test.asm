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
	ret
test:
	mov		rbx, 2
	mov		[abc], rbx
	ret

	call printint
	mov		rsi, 0
	sub		rsp, 32
	call	exit

	section .bss
main:		dd 0
abc:		dq 0
test:		dd 0
