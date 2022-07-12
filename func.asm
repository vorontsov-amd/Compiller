global main
extern printf, scanf, putchar

section .data
const_0: dq 0.000000
const_1: dq 0.000000
const_2: dq 0.000000
const_3: dq 2.000000
const_4: dq 0.000000
const_5: dq 0.000000
const_6: dq 1.000000
const_7: dq 0.000000
const_8: dq 0.000000
const_9: dq 0.000000
const_10: dq 1.000000
const_11: dq 0.000000
const_12: dq 0.000000
const_13: dq 0.000000
const_14: dq 1.000000
const_15: dq 2.000000
const_16: dq 2.000000
const_17: dq 2.000000
const_18: dq 4.000000
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
%define func_a [rbp - 8]
		fld		qword [const_0]
		fstp	qword func_a
%define func_b [rbp - 16]
		fld		qword [const_1]
		fstp	qword func_b
%define func_c [rbp - 24]
		fld		qword [const_2]
		fstp	qword func_c
		mov		rdi, scan_double
		lea		rsi, func_a
		mov		eax, 1
		call	scanf
		mov		rdi, scan_double
		lea		rsi, func_b
		mov		eax, 1
		call	scanf
		mov		rdi, scan_double
		lea		rsi, func_c
		mov		eax, 1
		call	scanf
%define func_num_sol [rbp - 32]
		lea		rax, func_c
		push	rax
		lea		rax, func_b
		push	rax
		lea		rax, func_a
		push	rax
		call	NumSol
		add		rsp, 24
		movsd	qword [result], xmm0
		fld		qword [result]
		fstp	qword func_num_sol
		lea		rax, func_num_sol
		push	rax
		lea		rax, func_c
		push	rax
		lea		rax, func_b
		push	rax
		lea		rax, func_a
		push	rax
		call	Solve
		add		rsp, 32
.ret_func:
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
		fld		qword Solve_num_sol
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_3]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if0else
%define Solve_x1 [rbp - 40]
		fld		qword [const_4]
		fstp	qword Solve_x1
%define Solve_x2 [rbp - 48]
		fld		qword [const_5]
		fstp	qword Solve_x2
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
		jmp		.if0end
.if0else:
		fld		qword Solve_num_sol
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_6]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if1end
%define Solve_x [rbp - 56]
		fld		qword [const_7]
		fstp	qword Solve_x
		fld		qword Solve_a
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_8]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		je		.if2else
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
		jmp		.if2end
.if2else:
		lea		rax, Solve_x
		push	rax
		lea		rax, Solve_c
		push	rax
		lea		rax, Solve_b
		push	rax
		call	SolveLinear
		add		rsp, 24
.if2end:
		mov		rdi, print_double
		movsd	xmm0, qword Solve_x
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
.if1end:
.if0end:
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
		fld		qword NumSol_a
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_9]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if3else
		fld		qword [const_10]
		fstp		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_NumSol
		jmp		.if3end
.if3else:
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
		fld		qword [result]
		fstp	qword NumSol_D
		fld		qword NumSol_D
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_11]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jae		.if4else
		fld		qword [const_12]
		fstp		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_NumSol
		jmp		.if4end
.if4else:
		fld		qword NumSol_D
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_13]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if5else
		fld		qword [const_14]
		fstp		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_NumSol
		jmp		.if5end
.if5else:
		fld		qword [const_15]
		fstp		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_NumSol
.if5end:
.if4end:
.if3end:
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
		fld		qword [result]
		fstp	qword SolveSquare_D
		fldz
		fld		qword SolveSquare_b
		fsubp	st1
		fld		qword SolveSquare_D
		fsqrt
		fsubp	st1
		fld		qword [const_16]
		fld		qword SolveSquare_a
		fmulp	st1
		fdivp	st1
		fstp	qword SolveSquare_x1
		fldz
		fld		qword SolveSquare_b
		fsubp	st1
		fld		qword SolveSquare_D
		fsqrt
		faddp	st1
		fld		qword [const_17]
		fld		qword SolveSquare_a
		fmulp	st1
		fdivp	st1
		fstp	qword SolveSquare_x2
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
		fld		qword SolveLinear_b
		fld		qword SolveLinear_a
		fdivp	st1
		fsubp	st1
		fstp	qword SolveLinear_x
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
		fld		qword CalcDiscr_b
		fld		qword CalcDiscr_b
		fmulp	st1
		fld		qword [const_18]
		fld		qword CalcDiscr_a
		fmulp	st1
		fld		qword CalcDiscr_c
		fmulp	st1
		fsubp	st1
		fstp		qword [result]
		movsd	xmm0, qword [result]
		jmp		.ret_CalcDiscr
.ret_CalcDiscr:
		leave
		ret
