	global  main
	extern  exit

%macro pushaq 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popaq 0
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rbp
	pop rdi
	pop rsi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
%endmacro

	section .text
main:
	mov		rax, 3
	mov		[abc_], rax
	pushaq
	call		test_
	popaq
	mov		rsi, 0
	sub		rsp, 32
	call	exit

test_:
	mov		rax, 2
	mov		[abc_], rax
	mov		rax, 4
	mov		rax, rax
	ret
	ret

	section .bss
abc_:				resq 1
