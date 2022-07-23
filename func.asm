global main
extern printf, scanf, putchar

section .data
buffer: dq 0.0
str: dd 0
section .rodata
const_0: dq 0.000000
section .text
main:
		finit
		call	func
		mov		rax, 60
		xor		rdi, rdi
		syscall

func:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
		mov		rdi, print_double
		movsd	xmm0, qword [rbp - 16]
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
.ret_func:
		leave
		ret
