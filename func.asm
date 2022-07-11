global main
extern printf, scanf, putchar

section .data
const_0: dq 1.000000
const_1: dq 10.000000
const_2: dq 2.000000
const_3: dq 20.000000
print_double: db '%lg ', 0x0
scan_double: db '%lg', 0x0
section .bss
result: resq 1
section .text
%define func main

func:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		finit
%define func_x [rbp - 8]
		mov		rdi, scan_double
		lea		rsi, func_x
		mov		eax, 1
		call	scanf
		fld		qword func_x
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_0]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if0else
		fld		qword [const_1]
		fstp	qword func_x
		jmp		.if0end
.if0else:
		fld		qword func_x
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_2]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		jne		.if1end
		fld		qword [const_3]
		fstp	qword func_x
.if1end:
.if0end:
		mov		rdi, print_double
		movsd	xmm0, qword func_x
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
.ret_func:
		leave
		ret
