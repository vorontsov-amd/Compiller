global main
extern printf, scanf, putchar

section .data
buffer: dq 0.0
section .rodata
const_0: dq 0.000000
const_1: dq 23.000000
const_2: dq 10.000000
const_3: dq 11.000000
const_4: dq 11.000000
const_5: dq 11.000000
const_6: dq 5.000000
const_7: dq 10.000000
const_8: dq 11.000000
const_9: dq 0.000000
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
		movsd	qword [rbp - 24], xmm0
		push	qword [rbp - 24]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if0else
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [rbp - 24]
		push	qword [rbp - 32]
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [rbp - 24]
		push	qword [rbp - 32]
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [rbp - 24]
		push	qword [rbp - 32]
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		jmp		.if0end
.if0else:
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
.if0end:
.ret_func:
		leave
		ret
