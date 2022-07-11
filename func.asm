global main
extern printf, scanf, putchar

section .data
const_0: dq 5.000000
const_1: dq 10.000000
print_double: db '%lg ', 0x0
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
		fld		qword [const_0]
		fstp	qword func_x
%define func_y [rbp - 16]
		fld		qword [const_1]
		fstp	qword func_y
		lea		rax, func_y
		push	rax
		lea		rax, func_x
		push	rax
		call	exc
		add		rsp, 16
		mov		rdi, print_double
		movsd	xmm0, qword func_x
		mov		eax, 1
		call	printf
		mov		rdi, print_double
		movsd	xmm0, qword func_y
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
		leave
		ret

exc:
%define exc_y_ptr [rbp + 24]
%define exc_y [rbp - 16]
%define exc_x_ptr [rbp + 16]
%define exc_x [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, exc_y_ptr
		movsd		xmm0, qword [rax]
		movsd		exc_y, xmm0
		mov		rax, exc_x_ptr
		movsd		xmm0, qword [rax]
		movsd		exc_x, xmm0
%define exc_t [rbp - 24]
		fld		qword exc_x
		fstp	qword exc_t
		fld		qword exc_y
		fstp	qword exc_x
		fld		qword exc_t
		fstp	qword exc_y
		mov		rdi, print_double
		movsd	xmm0, qword exc_x
		mov		eax, 1
		call	printf
		mov		rdi, print_double
		movsd	xmm0, qword exc_y
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
		mov		rax, exc_y_ptr
		movsd	xmm0, qword exc_y
		movsd	qword [rax], xmm0
		mov		rax, exc_x_ptr
		movsd	xmm0, qword exc_x
		movsd	qword [rax], xmm0
		leave
		ret
