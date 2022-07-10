global main
extern printf, scanf

section .data
const_0: dq 10.000000
const_1: dq 5.000000
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
		fld		func_x
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		func_y
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		je		.If0end
		push	func_y
		push	func_x
		call	print
		add		rsp, 16
.If0end:
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
