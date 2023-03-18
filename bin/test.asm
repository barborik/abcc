	global  main

	section .text
main:
	sub		rsp, 8
	call	ret2_
	mov		rax, rax
	mov		[rsp + 0], rax
	call	ret2_
	mov		rdi, rax
	mov		rsi, 8
	add		rdi, rsi
	call	test_
	mov		rbx, 0
	mov		rax, rbx
	add		rsp, 8
	ret

ret2_:
	mov		rax, 8
	mov		rax, rax
	add		rsp, 0
	ret

test_:
	push	rdi
	mov		rdi, [rsp + 0]
	call	print1i
	add		rsp, 8
	ret

	extern print1i

	section .bss
