global main
extern printf, scanf, putchar

section .data
buffer: dq 0.0
str: dd 0
section .rodata
const_0: dq 3.000000
const_1: dq 1.000000
const_2: dq 1.000000
const_3: dq 1.000000
print_double: db '%lg ', 0x0
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
		movsd	qword [rbp - 8], xmm0
		lea		rax, [rbp - 8]
		push	rax
		call	fuck
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
		mov		rdi, print_double
		movsd	xmm0, qword [rbp - 16]
		mov		eax, 1
		sub		rsp, 8
		call	printf
		mov		rdi, 10d
		call	putchar
		add  	rsp, 8
.ret_func:
		leave
		ret

fuck:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_1]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if0else
		push	qword [const_2]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fuck
		jmp		.if0end
.if0else:
		push	qword [rbp - 8]
		push	qword [const_3]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
		push	qword [rbp - 8]
		lea		rax, [rbp - 16]
		push	rax
		call	func
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fuck
.if0end:
.ret_fuck:
		leave
		ret
