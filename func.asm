global main
extern printf, scanf, putchar

section .data
const_0: dq 1.000000
const_1: dq 1.000000
const_2: dq 1.000000
const_3: dq 1.000000
const_4: dq 1.000000
const_5: dq 1.000000
const_6: dq 1.000000
const_7: dq 2.000000
const_8: dq 1.000000
const_9: dq 1.000000
const_10: dq 2.000000
const_11: dq 0.000000
const_12: dq 0.000000
const_13: dq 0.000000
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
scan_double: db '%lg', 0x0
section .bss
result: resq 1
section .text
%define func main

func:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		finit
		mov		rdi, scan_double
		lea		rsi, [rbp - 8]
		mov		eax, 1
		call	scanf
		push	qword [const_0]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 16], xmm0
		jmp		.while0test
.while0loop:
		lea		rax, [rbp - 16]
		push	rax
		call	fib
		add		rsp, 8
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 24], xmm0
		mov		rdi, print_double
		movsd	xmm0, qword [rbp - 24]
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
		push	qword [rbp - 16]
		push	qword [const_1]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		addsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 16], xmm0
.while0test:
		push	qword [rbp - 16]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.while0loop
.ret_func:
		leave
		ret

fuck:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_2]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if0else
		push	qword [const_3]
		pop		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_fuck
		jmp		.if0end
.if0else:
		push	qword [rbp - 8]
		push	qword [const_4]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 16], xmm0
		push	qword [rbp - 8]
		lea		rax, [rbp - 16]
		push	rax
		call	fuck
		add		rsp, 8
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
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
		movsd	[rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_5]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if1else
		push	qword [const_6]
		pop		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_fib
		jmp		.if1end
.if1else:
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_7]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if2else
		push	qword [const_8]
		pop		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_fib
		jmp		.if2end
.if2else:
		push	qword [rbp - 8]
		push	qword [const_9]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 16], xmm0
		push	qword [rbp - 8]
		push	qword [const_10]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 24], xmm0
		lea		rax, [rbp - 24]
		push	rax
		call	fib
		add		rsp, 8
		movsd	qword [result], xmm0
		push	qword [result]
		lea		rax, [rbp - 16]
		push	rax
		call	fib
		add		rsp, 8
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		addsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_fib
.if2end:
.if1end:
.ret_fib:
		leave
		ret

func1:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		push	qword [const_11]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 8], xmm0
		push	qword [const_12]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 16], xmm0
		push	qword [const_13]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 24], xmm0
		mov		rdi, scan_double
		lea		rsi, [rbp - 8]
		mov		eax, 1
		call	scanf
		mov		rdi, scan_double
		lea		rsi, [rbp - 16]
		mov		eax, 1
		call	scanf
		mov		rdi, scan_double
		lea		rsi, [rbp - 24]
		mov		eax, 1
		call	scanf
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	NumSol
		add		rsp, 24
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
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
.ret_func1:
		leave
		ret

Solve:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 64
		mov		rax, [rbp + 40]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 32], xmm0
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 8], xmm0
		push	qword [rbp - 32]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_14]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if3else
		push	qword [const_15]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 40], xmm0
		push	qword [const_16]
		pop		qword [result]
		movsd	xmm0, qword [result]
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
		mov		rdi, print_double
		movsd	xmm0, qword [rbp - 40]
		mov		eax, 1
		call	printf
		mov		rdi, print_double
		movsd	xmm0, qword [rbp - 48]
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
		jmp		.if3end
.if3else:
		push	qword [rbp - 32]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_17]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if4end
		push	qword [const_18]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 56], xmm0
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_19]
		pop		qword [result]
		movsd	xmm1, qword [result]
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
		mov		rdi, print_double
		movsd	xmm0, qword [rbp - 56]
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
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
		movsd	[rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_20]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if6else
		push	qword [const_21]
		pop		qword [result]
		movsd	xmm0, qword [result]
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
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 32], xmm0
		push	qword [rbp - 32]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_22]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jae		.if7else
		push	qword [const_23]
		pop		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_NumSol
		jmp		.if7end
.if7else:
		push	qword [rbp - 32]
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_24]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if8else
		push	qword [const_25]
		pop		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_NumSol
		jmp		.if8end
.if8else:
		push	qword [const_26]
		pop		qword [result]
		movsd	xmm0, qword [result]
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
		movsd	[rbp - 40], xmm0
		mov		rax, [rbp + 40]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 32], xmm0
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 8], xmm0
		lea		rax, [rbp - 24]
		push	rax
		lea		rax, [rbp - 16]
		push	rax
		lea		rax, [rbp - 8]
		push	rax
		call	CalcDiscr
		add		rsp, 24
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 48], xmm0
		fldz
		fstp	qword [result]
		push	qword [result]
		push	qword [rbp - 16]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword [rbp - 48]
		pop		qword [result]
		fld	qword [result]
		fsqrt
		fstp	qword [result]
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword [const_27]
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		divsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword [rbp - 32], xmm0
		fldz
		fstp	qword [result]
		push	qword [result]
		push	qword [rbp - 16]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword [rbp - 48]
		pop		qword [result]
		fld	qword [result]
		fsqrt
		fstp	qword [result]
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		addsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword [const_28]
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		divsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
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
		movsd	[rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 8], xmm0
		fldz
		fstp	qword [result]
		push	qword [result]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		divsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
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
		movsd	[rbp - 24], xmm0
		mov		rax, [rbp + 24]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 16], xmm0
		mov		rax, [rbp + 16]
		movsd	xmm0, qword [rax]
		movsd	[rbp - 8], xmm0
		push	qword [rbp - 16]
		push	qword [rbp - 16]
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword [const_29]
		push	qword [rbp - 8]
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword [rbp - 24]
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_CalcDiscr
.ret_CalcDiscr:
		leave
		ret
