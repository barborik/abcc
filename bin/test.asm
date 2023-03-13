	global  main
	extern  exit

	section .text
main:
	mov		rax, 1
	mov		[abc_], rax
	mov		rsi, 0
	sub		rsp, 32
	call	exit

test_:
	mov		rbx, 2
	mov		[abc_], rbx
	ret
test2_:
	mov		rcx, 3
	mov		[abc_], rcx
	ret

	section .bss
abc_:				resq 1
