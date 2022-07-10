global main
extern printf, scanf

section .data
const_0: dq 0.000000
const_1: dq 5.000000
const_2: dq 10.000000
const_3: dq 100.000000
doublestr: db '%lg %lg', 0x0
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
		push	func_y
		push	func_x
		call	f
		add		rsp, 16
		push	func_y
		push	func_x
		call	print
		add		rsp, 16
		leave
		ret

f:
%define f_y qword [rbp + 24]
%define f_x qword [rbp + 16]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 0
		fld		qword [const_2]
		fstp	f_x
		fld		qword [const_3]
		fstp	f_y
		leave
		ret

print:
%define print_y qword [rbp + 24]
%define print_x qword [rbp + 16]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 0
		mov		rdi,  doublestr
		movsd 	xmm0, print_x
		movsd	xmm1, print_y
		mov		eax, 2
		call 	printf
		leave
		ret