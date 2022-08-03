global main
extern printf, scanf, putchar

section .data
buffer: dq 0.0
str: times 32 db 0
section .rodata
const_0: dq 0.000000
const_1: dq 0.000000
const_2: dq 0.000000
const_3: dq 1.000000
const_4: dq 1.000000
const_5: dq 1.000000
const_6: dq 1.000000
const_7: dq 1.000000
const_8: dq 1.000000
const_9: dq 1.000000
const_10: dq 2.000000
const_11: dq 1.000000
const_12: dq 1.000000
const_13: dq 2.000000
const_14: dq 2.000000
const_15: dq 0.000000
const_16: dq 0.000000
const_17: dq 1.000000
const_18: dq 0.000000
const_19: dq 0.000000
const_20: dq 0.000000
const_21: dq 1.000000
const_22: dq 0.000000
const_23: dq 0.000000
const_24: dq 0.000000
const_25: dq 1.000000
const_26: dq 2.000000
const_27: dq 2.000000
const_28: dq 2.000000
const_29: dq 4.000000
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
		sub		rsp, 32
		push	qword [const_0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 8], xmm0
		push	qword [const_1]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
		push	qword [const_2]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rdi, str
		syscall
		mov		rdi, str
		call		atod
		movsd		qword [rbp - 8], xmm0
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rdi, str
		syscall
		mov		rdi, str
		call		atod
		movsd		qword [rbp - 16], xmm0
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rdi, str
		syscall
		mov		rdi, str
		call		atod
		movsd		qword [rbp - 24], xmm0
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	NumSol
		add		rsp, 24
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		lea		rax, [rbp - 32]
		push	rax
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	Solve
		add		rsp, 32
.ret_func:
		leave
		ret

func1:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rdi, str
		syscall
		mov		rdi, str
		call		atod
		movsd		qword [rbp - 8], xmm0
		push	qword [const_3]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
		jmp		.while0test
.while0loop:
		lea		rax, [rbp - 16]
		push	rax
		call	fib
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
		mov		rdi, str
		movsd	xmm0, qword [rbp - 24]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		push	qword [rbp - 16]
		push	qword [const_4]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
.while0test:
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.while0loop
.ret_func1:
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
		push	qword [const_5]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if0else
		push	qword [const_6]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fuck
		jmp		.if0end
.if0else:
		push	qword [rbp - 8]
		push	qword [const_7]
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
		call	fuck
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

fib:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_8]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if1else
		push	qword [const_9]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fib
		jmp		.if1end
.if1else:
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_10]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if2else
		push	qword [const_11]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fib
		jmp		.if2end
.if2else:
		push	qword [rbp - 8]
		push	qword [const_12]
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
		push	qword [const_13]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
		lea		rax, [rbp - 24]
		push	rax
		call	fib
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		lea		rax, [rbp - 16]
		push	rax
		call	fib
		add		rsp, 8
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
		jmp		.ret_fib
.if2end:
.if1end:
.ret_fib:
		leave
		ret

Solve:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 64
		mov		rax, [rbp + 40]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 32], xmm0
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_14]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if3else
		push	qword [const_15]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 40], xmm0
		push	qword [const_16]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 48], xmm0
		lea		rax, [rbp - 48]
		push	rax
		lea		rax, [rbp - 40]
		push	rax
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	SolveSquare
		add		rsp, 40
		mov		rdi, str
		movsd	xmm0, qword [rbp - 40]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		mov		rdi, str
		movsd	xmm0, qword [rbp - 48]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		jmp		.if3end
.if3else:
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_17]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if4end
		push	qword [const_18]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 56], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_19]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if5else
		lea		rax, [rbp - 56]
		push	rax
		lea		rax, [rbp - 56]
		push	rax
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	SolveSquare
		add		rsp, 40
		jmp		.if5end
.if5else:
		lea		rax, [rbp - 56]
		push	rax
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		call	SolveLinear
		add		rsp, 24
.if5end:
		mov		rdi, str
		movsd	xmm0, qword [rbp - 56]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
.if4end:
.if3end:
.ret_Solve:
		leave
		ret

NumSol:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_20]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if6else
		push	qword [const_21]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
		jmp		.if6end
.if6else:
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	CalcDiscr
		add		rsp, 24
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_22]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jae		.if7else
		push	qword [const_23]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
		jmp		.if7end
.if7else:
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_24]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if8else
		push	qword [const_25]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
		jmp		.if8end
.if8else:
		push	qword [const_26]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
.if8end:
.if7end:
.if6end:
.ret_NumSol:
		leave
		ret

SolveSquare:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 48
		mov		rax, [rbp + 48]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 40], xmm0
		mov		rax, [rbp + 40]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 32], xmm0
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 8], xmm0
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	CalcDiscr
		add		rsp, 24
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 48], xmm0
		fldz
		fstp	qword [buffer]
		push	qword [buffer]
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [rbp - 48]
		pop		qword [buffer]
		sqrtsd	xmm0, [buffer]
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [const_27]
		push	qword [rbp - 8]
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
		divsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		fldz
		fstp	qword [buffer]
		push	qword [buffer]
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [rbp - 48]
		pop		qword [buffer]
		sqrtsd	xmm0, [buffer]
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [const_28]
		push	qword [rbp - 8]
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
		divsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 40], xmm0
.ret_SolveSquare:
		mov		rax, [rbp + 48]
		movsd	xmm0, qword [rbp - 40]
		movsd	qword [rax], xmm0
		mov		rax, [rbp + 40]
		movsd	xmm0, qword [rbp - 32]
		movsd	qword [rax], xmm0
		leave
		ret

SolveLinear:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 8], xmm0
		fldz
		fstp	qword [buffer]
		push	qword [buffer]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		divsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
.ret_SolveLinear:
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rbp - 24]
		movsd	qword [rax], xmm0
		leave
		ret

CalcDiscr:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 16]
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [const_29]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [rbp - 24]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_CalcDiscr
.ret_CalcDiscr:
		leave
		ret
