global main
extern printf, scanf, putchar

section .data
const_0: dq 0.000000
const_1: dq 5.000000
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
%define func_x qword [rbp - 8]
		fld		qword [const_0]
		fstp	func_x
%define func_y qword [rbp - 16]
		fld		qword [const_1]
		fstp	func_y
		mov		rdi, print_double
		movsd	xmm0, func_x
		mov		eax, 1
		call	printf
		mov		rdi, print_double
		movsd	xmm0, func_y
		mov		eax, 1
		call	printf
		mov		rdi, 10d
		call	putchar
		leave
		ret
