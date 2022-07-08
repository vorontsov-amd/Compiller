global main
extern printf, scanf

section .data
doublestr: db '%lg', 0x0

section .text
%define func main

func:
%define func_arg4 qword [rbp + 40]
%define func_arg3 qword [rbp + 32]
%define func_argv qword [rbp + 24]
%define func_argc qword [rbp + 16]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
drink:
%define drink_pivo qword [rbp + 16]
		push	rbp
		mov		rbp, rsp
		sub		rsp, 0
exit:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16