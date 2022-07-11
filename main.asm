global main
extern printf, scanf, putchar

section .data
const_0: dq 8.000000
const_1: dq 0.000000
const_2: dq 0.000000
print_double: db '%lg ', 0x0
section .bss
result: resq 1
section .text
%define main main

main:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		finit
%define main_x qword [rbp - 8]
%define main_y qword [rbp - 16]
		lea	rax, main_b
		push	rax
		lea	rax, main_a
		push	rax
		lea	rax, main_z
		push	rax
		lea	rax, main_y
		push	rax
		lea	rax, main_x
		push	rax
		call	lox
		add		rsp, 40
		lea	rax, main_y
		push	rax
		lea	rax, main_x
		push	rax
		call	fuck
		add		rsp, 16
		leave
		ret

fuck:
%define fuck_b_ptr qword [rbp + 48]
%define fuck_a_ptr qword [rbp + 40]
%define fuck_z_ptr qword [rbp + 32]
%define fuck_y_ptr qword [rbp + 24]
%define fuck_x_ptr qword [rbp + 16]
%define fuck_a qword [rbp - 24]
%define fuck_z qword [rbp - 16]
%define fuck_x qword [rbp - 8]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 48
		movsd	xmm0, fuck_a_ptr
		movsd	fuck_a, xmm0
		movsd	xmm0, fuck_z_ptr
		movsd	fuck_z, xmm0
		movsd	xmm0, fuck_x_ptr
		movsd	fuck_x, xmm0
%define fuck_papper qword [rbp - 32]
		fld		qword [const_0]
		fstp	fuck_papper
		fld		fuck_x
		fstp	qword [result]
		movsd	xmm0, qword [result]
		fld		qword [const_1]
		fstp	qword [result]
		movsd	xmm1, qword [result]
		comisd	xmm0, xmm1
		je		.if0end
%define fuck_sex qword [rbp - 40]
		fld		qword [const_2]
		fstp	fuck_sex
.If0end:
		leave
		ret
