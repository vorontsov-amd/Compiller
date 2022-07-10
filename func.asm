global main
extern printf, scanf

section .data
const_0: dq 10.000000
const_1: dq 5.000000
const_2: dq 4.000000
const_3: dq 5.000000
const_4: dq 3.000000
const_5: dq 7.000000
const_6: dq 0.000000
doublestr: db '%lg %lg %lg %lg', 0x0
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
		call	Translate
		add		rsp, 16
		leave
		ret

Translate:
%define Translate_y qword [rbp + 24]
%define Translate_x qword [rbp + 16]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		fld		qword [const_2]
		fstp	Translate_x
		fld		Translate_x
		fld		qword [const_3]
		faddp		st1
		fstp	Translate_y
%define Translate_z qword [rbp - 8]
		fld		Translate_x
		fld		Translate_y
		fmulp		st1
		fld		qword [const_4]
		fsubp		st1
		fld		qword [const_5]
		fdivp		st1
		fstp	Translate_z
%define Translate_c qword [rbp - 16]
		fld		qword [const_6]
		fstp	Translate_c
		push	Translate_c
		push	Translate_z
		push	Translate_y
		push	Translate_x
		call	print
		add		rsp, 32
		leave
		ret

print:
%define print_c qword [rbp + 40]
%define print_z qword [rbp + 32]
%define print_y qword [rbp + 24]
%define print_x qword [rbp + 16]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 0
		movsd	xmm0, print_x
		movsd	xmm1, print_y
		movsd 	xmm2, print_z
		movsd 	xmm3, print_c
		mov 	rdi, doublestr
		mov 	eax, 4
		call	printf
		leave
		ret
