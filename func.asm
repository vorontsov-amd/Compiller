global _start
extern dtoa, atod, pow

section .data
buffer: dq 0.0
str: times 32 db 0
section .rodata
const_0: dq 2.000000
print_double: db '%lg ', 0x0
section .text
_start:
		finit
		call	func
		mov		rax, 60
		xor		rdi, rdi
		syscall

func:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rdi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 8], 