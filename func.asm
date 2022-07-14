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
%define func_x [rbp - 8]
		mov		rdi, scan_double
		lea		rsi, func_x
		mov		eax, 1
		call	scanf
%define func_i [rbp - 16]
		push	qword [const_0]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword func_i, xmm0
		jmp		.while0test
.while0loop:
%define func_t [rbp - 24]
		lea		rax, func_i
		push	rax
		call	fib
		add		rsp, 8
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword func_t, xmm0
		mov		rdi, print_double
		movsd	xmm0, qword func_t
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
		push	qword func_i
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
		movsd	qword func_i, xmm0
.while0test:
		push	qword func_i
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword func_x
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.while0loop
.ret_func:
		leave
		ret

fuck:
%define fuck_x_ptr [rbp + 16]
%define fuck_x [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		mov		rax, fuck_x_ptr
		movsd	xmm0, qword [rax]
		movsd	fuck_x, xmm0
		push	qword fuck_x
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
%define fuck_t [rbp - 16]
		push	qword fuck_x
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
		movsd	qword fuck_t, xmm0
		push	qword fuck_x
		lea		rax, fuck_t
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
%define fib_x_ptr [rbp + 16]
%define fib_x [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, fib_x_ptr
		movsd	xmm0, qword [rax]
		movsd	fib_x, xmm0
		push	qword fib_x
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
		push	qword fib_x
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
%define fib_x1 [rbp - 16]
		push	qword fib_x
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
		movsd	qword fib_x1, xmm0
%define fib_x2 [rbp - 24]
		push	qword fib_x
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
		movsd	qword fib_x2, xmm0
		lea		rax, fib_x2
		push	rax
		call	fib
		add		rsp, 8
		movsd	qword [result], xmm0
		push	qword [result]
		lea		rax, fib_x1
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
%define func1_a [rbp - 8]
		push	qword [const_11]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword func1_a, xmm0
%define func1_b [rbp - 16]
		push	qword [const_12]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword func1_b, xmm0
%define func1_c [rbp - 24]
		push	qword [const_13]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword func1_c, xmm0
		mov		rdi, scan_double
		lea		rsi, func1_a
		mov		eax, 1
		call	scanf
		mov		rdi, scan_double
		lea		rsi, func1_b
		mov		eax, 1
		call	scanf
		mov		rdi, scan_double
		lea		rsi, func1_c
		mov		eax, 1
		call	scanf
%define func1_num_sol [rbp - 32]
		lea		rax, func1_c
		push	rax
		lea		rax, func1_b
		push	rax
		lea		rax, func1_a
		push	rax
		call	NumSol
		add		rsp, 24
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword func1_num_sol, xmm0
		lea		rax, func1_num_sol
		push	rax
		lea		rax, func1_c
		push	rax
		lea		rax, func1_b
		push	rax
		lea		rax, func1_a
		push	rax
		call	Solve
		add		rsp, 32
.ret_func1:
		leave
		ret

Solve:
%define Solve_num_sol_ptr [rbp + 40]
%define Solve_num_sol [rbp - 32]
%define Solve_c_ptr [rbp + 32]
%define Solve_c [rbp - 24]
%define Solve_b_ptr [rbp + 24]
%define Solve_b [rbp - 16]
%define Solve_a_ptr [rbp + 16]
%define Solve_a [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 64
		mov		rax, Solve_num_sol_ptr
		movsd	xmm0, qword [rax]
		movsd	Solve_num_sol, xmm0
		mov		rax, Solve_c_ptr
		movsd	xmm0, qword [rax]
		movsd	Solve_c, xmm0
		mov		rax, Solve_b_ptr
		movsd	xmm0, qword [rax]
		movsd	Solve_b, xmm0
		mov		rax, Solve_a_ptr
		movsd	xmm0, qword [rax]
		movsd	Solve_a, xmm0
		push	qword Solve_num_sol
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_14]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if3else
%define Solve_x1 [rbp - 40]
		push	qword [const_15]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword Solve_x1, xmm0
%define Solve_x2 [rbp - 48]
		push	qword [const_16]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword Solve_x2, xmm0
		lea		rax, Solve_x2
		push	rax
		lea		rax, Solve_x1
		push	rax
		lea		rax, Solve_c
		push	rax
		lea		rax, Solve_b
		push	rax
		lea		rax, Solve_a
		push	rax
		call	SolveSquare
		add		rsp, 40
		mov		rdi, print_double
		movsd	xmm0, qword Solve_x1
		mov		eax, 1
		call	printf
		mov		rdi, print_double
		movsd	xmm0, qword Solve_x2
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
		jmp		.if3end
.if3else:
		push	qword Solve_num_sol
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_17]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if4end
%define Solve_x [rbp - 56]
		push	qword [const_18]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword Solve_x, xmm0
		push	qword Solve_a
		pop		qword [result]
		movsd	xmm0, qword [result]
		push	qword [const_19]
		pop		qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		je		.if5else
		lea		rax, Solve_x
		push	rax
		lea		rax, Solve_x
		push	rax
		lea		rax, Solve_c
		push	rax
		lea		rax, Solve_b
		push	rax
		lea		rax, Solve_a
		push	rax
		call	SolveSquare
		add		rsp, 40
		jmp		.if5end
.if5else:
		lea		rax, Solve_x
		push	rax
		lea		rax, Solve_c
		push	rax
		lea		rax, Solve_b
		push	rax
		call	SolveLinear
		add		rsp, 24
.if5end:
		mov		rdi, print_double
		movsd	xmm0, qword Solve_x
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
%define NumSol_c_ptr [rbp + 32]
%define NumSol_c [rbp - 24]
%define NumSol_b_ptr [rbp + 24]
%define NumSol_b [rbp - 16]
%define NumSol_a_ptr [rbp + 16]
%define NumSol_a [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, NumSol_c_ptr
		movsd	xmm0, qword [rax]
		movsd	NumSol_c, xmm0
		mov		rax, NumSol_b_ptr
		movsd	xmm0, qword [rax]
		movsd	NumSol_b, xmm0
		mov		rax, NumSol_a_ptr
		movsd	xmm0, qword [rax]
		movsd	NumSol_a, xmm0
		push	qword NumSol_a
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
%define NumSol_D [rbp - 32]
		lea		rax, NumSol_c
		push	rax
		lea		rax, NumSol_b
		push	rax
		lea		rax, NumSol_a
		push	rax
		call	CalcDiscr
		add		rsp, 24
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword NumSol_D, xmm0
		push	qword NumSol_D
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
		push	qword NumSol_D
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
%define SolveSquare_x2_ptr [rbp + 48]
%define SolveSquare_x2 [rbp - 40]
%define SolveSquare_x1_ptr [rbp + 40]
%define SolveSquare_x1 [rbp - 32]
%define SolveSquare_c_ptr [rbp + 32]
%define SolveSquare_c [rbp - 24]
%define SolveSquare_b_ptr [rbp + 24]
%define SolveSquare_b [rbp - 16]
%define SolveSquare_a_ptr [rbp + 16]
%define SolveSquare_a [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 48
		mov		rax, SolveSquare_x2_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveSquare_x2, xmm0
		mov		rax, SolveSquare_x1_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveSquare_x1, xmm0
		mov		rax, SolveSquare_c_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveSquare_c, xmm0
		mov		rax, SolveSquare_b_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveSquare_b, xmm0
		mov		rax, SolveSquare_a_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveSquare_a, xmm0
%define SolveSquare_D [rbp - 48]
		lea		rax, SolveSquare_c
		push	rax
		lea		rax, SolveSquare_b
		push	rax
		lea		rax, SolveSquare_a
		push	rax
		call	CalcDiscr
		add		rsp, 24
		movsd	qword [result], xmm0
		push	qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		movsd	qword SolveSquare_D, xmm0
		fldz
		fstp	qword [result]
		push	qword [result]
		push	qword SolveSquare_b
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword SolveSquare_D
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
		push	qword SolveSquare_a
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
		movsd	qword SolveSquare_x1, xmm0
		fldz
		fstp	qword [result]
		push	qword [result]
		push	qword SolveSquare_b
		pop		qword [result]
		movsd	xmm1, [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		subsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword SolveSquare_D
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
		push	qword SolveSquare_a
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
		movsd	qword SolveSquare_x2, xmm0
.ret_SolveSquare:
		mov		rax, SolveSquare_x2_ptr
		movsd	xmm0, qword SolveSquare_x2
		movsd	qword [rax], xmm0
		mov		rax, SolveSquare_x1_ptr
		movsd	xmm0, qword SolveSquare_x1
		movsd	qword [rax], xmm0
		leave
		ret

SolveLinear:
%define SolveLinear_x_ptr [rbp + 32]
%define SolveLinear_x [rbp - 24]
%define SolveLinear_b_ptr [rbp + 24]
%define SolveLinear_b [rbp - 16]
%define SolveLinear_a_ptr [rbp + 16]
%define SolveLinear_a [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, SolveLinear_x_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveLinear_x, xmm0
		mov		rax, SolveLinear_b_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveLinear_b, xmm0
		mov		rax, SolveLinear_a_ptr
		movsd	xmm0, qword [rax]
		movsd	SolveLinear_a, xmm0
		fldz
		fstp	qword [result]
		push	qword [result]
		push	qword SolveLinear_b
		push	qword SolveLinear_a
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
		movsd	qword SolveLinear_x, xmm0
.ret_SolveLinear:
		mov		rax, SolveLinear_x_ptr
		movsd	xmm0, qword SolveLinear_x
		movsd	qword [rax], xmm0
		leave
		ret

CalcDiscr:
%define CalcDiscr_c_ptr [rbp + 32]
%define CalcDiscr_c [rbp - 24]
%define CalcDiscr_b_ptr [rbp + 24]
%define CalcDiscr_b [rbp - 16]
%define CalcDiscr_a_ptr [rbp + 16]
%define CalcDiscr_a [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, CalcDiscr_c_ptr
		movsd	xmm0, qword [rax]
		movsd	CalcDiscr_c, xmm0
		mov		rax, CalcDiscr_b_ptr
		movsd	xmm0, qword [rax]
		movsd	CalcDiscr_b, xmm0
		mov		rax, CalcDiscr_a_ptr
		movsd	xmm0, qword [rax]
		movsd	CalcDiscr_a, xmm0
		push	qword CalcDiscr_b
		push	qword CalcDiscr_b
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword [const_29]
		push	qword CalcDiscr_a
		pop		qword [result]
		movsd	xmm1, qword [result]
		pop		qword [result]
		movsd	xmm0, qword [result]
		mulsd	xmm0, xmm1
		movsd	qword [result], xmm0
		push	qword [result]
		push	qword CalcDiscr_c
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
