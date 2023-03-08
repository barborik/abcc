	global  main
	extern  printf
	extern  exit

	section .bss
abc:		dq 0

	section .text
format:
	db		"%d", 10, 0
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
	mov	rax, [abc]
	mov		rbx, 1
	add		rax, rbx
	mov		[abc], rax
L1:
	mov	rbx, [abc]
	mov		rcx, 10
	cmp		rbx, rcx
	setl		bl
	and		rbx, 0xff
	cmp		rbx, 0
	je		L2
	mov	rdx, [abc]
	mov		(null), 1
	add		rdx, (null)
	mov		[abc], rdx
	jmp		L1
L2:

	call printint
	mov		rsi, 0
	sub		rsp, 32
	call	exit
