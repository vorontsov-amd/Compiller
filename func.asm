global _start
extern dtoa, atod, pow

section .data
buffer: dq 0.0
str: times 32 db 0
section .rodata
const_0: dq 1.000000
const_0: dq 2.000000
const_0: dq 3.000000
const_0: dq 1.000000
const_0: dq 3.000000
const_0: dq 1.000000
const_0: dq 2.000000
const_0: dq 1.000000
const_0: dq 1.000000
const_0: dq 1.000000
const_0: dq 1.000000
const_0: dq 1.000000
const_0: dq 1.000000
const_0: dq 1.000000
const_0: dq 1.000000
const_0: dq 2.000000
const_0: dq 1.000000
const_0: dq 2.000000
const_0: dq 1.000000
const_0: dq 2.000000
const_0: dq 0.000000
const_0: dq 0.000000
const_0: dq 1.000000
const_0: dq 0.000000
const_0: dq 0.000000
const_0: dq 0.000000
const_0: dq 1.000000
const_0: dq 0.000000
const_0: dq 0.000000
const_0: dq 0.000000
const_0: dq -1.000000
const_0: dq 0.000000
const_0: dq 1.000000
const_0: dq 0.000000
const_0: dq 0.000000
const_0: dq 0.000000
const_0: dq 1.000000
const_0: dq 2.000000
const_0: dq 2.000000
const_0: dq 2.000000
const_0: dq 2.000000
const_0: dq 4.000000
str_0: db 'Enter number test programm'
str_0_len: equ $-str_0
str_1: db 'SolveSquareEquation'
str_1_len: equ $-str_1
str_2: db 'fibonacci'
str_2_len: equ $-str_2
str_3: db 'factorial'
str_3_len: equ $-str_3
str_4: db 'try again'
str_4_len: equ $-str_4
str_5: db 'Done'
str_5_len: equ $-str_5
str_6: db 'enter a'
str_6_len: equ $-str_6
str_7: db 'enter b'
str_7_len: equ $-str_7
str_8: db 'enter c'
str_8_len: equ $-str_8
str_9: db 'enter fibonacci number'
str_9_len: equ $-str_9
str_10: db 'enter factorial number'
str_10_len: equ $-str_10
str_11: db 'answer'
str_11_len: equ $-str_11
str_12: db 'x1 ='
str_12_len: equ $-str_12
str_13: db 'x2 ='
str_13_len: equ $-str_13
str_14: db 'x ='
str_14_len: equ $-str_14
str_15: db 'no solutions'
str_15_len: equ $-str_15
str_16: db 'infity root'
str_16_len: equ $-str_16
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
		sub		rsp, 0
		mov		rsi, str_0
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 26
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [const_0]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rsi, str_1
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 19
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [const_1]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rsi, str_2
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 9
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [const_2]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rsi, str_3
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 9
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rsi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [rbp - 0], xmm0
		jmp		.while0test
.while0loop:
		mov		rsi, str_4
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 9
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rsi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 0], xmm0
.while0test:
		push	qword [rbp - 0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_3]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jb		.while0loop
		push	qword [rbp - 0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_4]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		ja		.while0loop
.while0end:
		push	qword [rbp - 0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_5]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if0start
		jmp		.if0else
.if0start:
		call	SolveSquareEquation
		add		rsp, 0
		jmp		.if0end
.if0else:
		push	qword [rbp - 0]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_6]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if1start
		jmp		.if1else
.if1start:
		call	fibonacci
		add		rsp, 0
		jmp		.if1end
.if1else:
		call	factorial
		add		rsp, 0
.if1end:
.if0end:
		mov		rsi, str_5
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 4
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
.ret_func:
		leave
		ret

SolveSquareEquation:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rsi, str_6
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 7
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rsi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 8], xmm0
		mov		rsi, str_7
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 7
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rsi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
		mov		rsi, str_8
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 7
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rsi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
		push	qword [rbp - 24]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		call	NumSol
		add		rsp, 24
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [rbp - 32]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 32]
		push	qword [rbp - 24]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		call	Solve
		add		rsp, 32
.ret_SolveSquareEquation:
		leave
		ret

fibonacci:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rsi, str_9
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 22
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rsi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 8], xmm0
		push	qword [const_7]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
		jmp		.while1test
.while1loop:
		push	qword [rbp - 16]
		call	fib
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
		push	qword [rbp - 16]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 24]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 16]
		push	qword [const_8]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 16], xmm0
.while1test:
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jbe		.while1loop
.while1end:
.ret_fibonacci:
		leave
		ret

factorial:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rsi, str_10
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 22
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rdx, 31
		xor		rax, rax
		xor		rdi, rdi
		mov		rsi, str
		syscall
		mov		rdi, str
		call	atod
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		push	qword [const_9]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 40], xmm0
		jmp		.while2test
.while2loop:
		push	qword [rbp - 40]
		call	fuck
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 48], xmm0
		push	qword [rbp - 40]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 48]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 40]
		push	qword [const_10]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 40], xmm0
.while2test:
		push	qword [rbp - 40]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jbe		.while2loop
.while2end:
.ret_factorial:
		leave
		ret

fuck:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		movsd	xmm0, qword [rbp + 16]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_11]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if2start
		jmp		.if2else
.if2start:
		push	qword [const_12]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fuck
		jmp		.if2end
.if2else:
		push	qword [rbp - 8]
		push	qword [rbp - 8]
		push	qword [const_13]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		call	fuck
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fuck
.if2end:
.ret_fuck:
		leave
		ret

fib:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 16
		movsd	xmm0, qword [rbp + 16]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_14]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if3start
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_15]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if3start
		jmp		.if3else
.if3start:
		push	qword [const_16]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fib
		jmp		.if3end
.if3else:
		push	qword [rbp - 8]
		push	qword [const_17]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		call	fib
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [rbp - 8]
		push	qword [const_18]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		call	fib
		add		rsp, 8
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_fib
.if3end:
.ret_fib:
		leave
		ret

Solve:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 64
		movsd	xmm0, qword [rbp + 40]
		movsd	qword [rbp - 32], xmm0
		movsd	xmm0, qword [rbp + 32]
		movsd	qword [rbp - 24], xmm0
		movsd	xmm0, qword [rbp + 24]
		movsd	qword [rbp - 16], xmm0
		movsd	xmm0, qword [rbp + 16]
		movsd	qword [rbp - 8], xmm0
		mov		rsi, str_11
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 6
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_19]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if4start
		jmp		.if4else
.if4start:
		push	qword [const_20]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 56], xmm0
		push	qword [const_21]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 64], xmm0
		lea		rax, [rbp - 64]
		push	rax
		lea		rax, [rbp - 56]
		push	rax
		push	qword [rbp - 24]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		call	SolveSquare
		add		rsp, 40
		mov		rsi, str_12
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 4
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 56]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rsi, str_13
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 4
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 64]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		jmp		.if4end
.if4else:
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_22]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if5start
		jmp		.if5else
.if5start:
		push	qword [const_23]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 72], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_24]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if6start
		jmp		.if6else
.if6start:
		lea		rax, [rbp - 72]
		push	rax
		lea		rax, [rbp - 72]
		push	rax
		push	qword [rbp - 24]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		call	SolveSquare
		add		rsp, 40
		jmp		.if6end
.if6else:
		lea		rax, [rbp - 72]
		push	rax
		push	qword [rbp - 24]
		push	qword [rbp - 16]
		call	SolveLinear
		add		rsp, 24
.if6end:
		mov		rsi, str_14
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 3
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		push	qword [rbp - 72]
		mov		rdi, str
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call	dtoa
		mov		rdx, rax
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, str
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		jmp		.if5end
.if5else:
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_25]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if7start
		jmp		.if7else
.if7start:
		mov		rsi, str_15
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 12
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		jmp		.if7end
.if7else:
		push	qword [rbp - 32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		fldz
		fstp	qword [buffer]
		push	qword [buffer]
		push	qword [const_26]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if8start
		jmp		.if8end
.if8start:
		mov		rsi, str_16
		mov		rdi, 1
		mov		rax, 1
		mov		rdx, 11
		syscall
		mov		rax, 32
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
		mov		rax, 10
		push	rax
		mov		rdx, 1
		mov		rdi, 1
		mov		rax, 1
		mov		rsi, rsp
		syscall
		pop		rax
.if8end:
.if7end:
.if5end:
.if4end:
.ret_Solve:
		leave
		ret

NumSol:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		movsd	xmm0, qword [rbp + 32]
		movsd	qword [rbp - 24], xmm0
		movsd	xmm0, qword [rbp + 24]
		movsd	qword [rbp - 16], xmm0
		movsd	xmm0, qword [rbp + 16]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_27]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if9else
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_28]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if9else
		push	qword [rbp - 24]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_29]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jne		.if9else
.if9start:
		push	qword [const_30]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
		jmp		.if9end
.if9else:
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_31]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if10start
		jmp		.if10else
.if10start:
		push	qword [const_32]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
		jmp		.if10end
.if10else:
		push	qword [rbp - 24]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		call	CalcDiscr
		add		rsp, 24
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 80], xmm0
		push	qword [rbp - 80]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_33]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		jb		.if11start
		jmp		.if11else
.if11start:
		push	qword [const_34]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
		jmp		.if11end
.if11else:
		push	qword [rbp - 80]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		push	qword [const_35]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		comisd	xmm0, xmm1
		je		.if12start
		jmp		.if12else
.if12start:
		push	qword [const_36]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
		jmp		.if12end
.if12else:
		push	qword [const_37]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_NumSol
.if12end:
.if11end:
.if10end:
.if9end:
.ret_NumSol:
		leave
		ret

SolveSquare:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 48
		mov		rax, [rbp + 48]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 40], xmm0
		mov		rax, [rbp + 40]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 32], xmm0
		movsd	xmm0, qword [rbp + 32]
		movsd	qword [rbp - 24], xmm0
		movsd	xmm0, qword [rbp + 24]
		movsd	qword [rbp - 16], xmm0
		movsd	xmm0, qword [rbp + 16]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 24]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		call	CalcDiscr
		add		rsp, 24
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 88], xmm0
		fldz
		fstp	qword [buffer]
		push	qword [buffer]
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [rbp - 88]
		pop		qword [buffer]
		sqrtsd	xmm0, [buffer]
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [const_38]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		divsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 32], xmm0
		fldz
		fstp	qword [buffer]
		push	qword [buffer]
		push	qword [rbp - 16]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [rbp - 88]
		pop		qword [buffer]
		sqrtsd	xmm0, [buffer]
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		addsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [const_39]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		divsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 40], xmm0
.ret_SolveSquare:
		mov		rax, [rbp + 48]
		movsd	xmm0, qword [rbp - 40]
		movsd	qword [rax], xmm0
		mov		rax, [rbp + 40]
		movsd	xmm0, qword [rbp - 32]
		movsd	qword [rax], xmm0
		leave
		ret

SolveLinear:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rax]
		movsd	qword [rbp - 24], xmm0
		movsd	xmm0, qword [rbp + 24]
		movsd	qword [rbp - 16], xmm0
		movsd	xmm0, qword [rbp + 16]
		movsd	qword [rbp - 8], xmm0
		fldz
		fstp	qword [buffer]
		push	qword [buffer]
		push	qword [rbp - 16]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		divsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		movsd	qword [rbp - 24], xmm0
.ret_SolveLinear:
		mov		rax, [rbp + 32]
		movsd	xmm0, qword [rbp - 24]
		movsd	qword [rax], xmm0
		leave
		ret

CalcDiscr:
		push	rbp
		mov		rbp, rsp
		sub		rsp, 32
		movsd	xmm0, qword [rbp + 32]
		movsd	qword [rbp - 24], xmm0
		movsd	xmm0, qword [rbp + 24]
		movsd	qword [rbp - 16], xmm0
		movsd	xmm0, qword [rbp + 16]
		movsd	qword [rbp - 8], xmm0
		push	qword [rbp - 16]
		push	qword [const_40]
		pop		qword [buffer]
		movsd	xmm1, [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		call		pow
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [const_41]
		push	qword [rbp - 8]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		push	qword [rbp - 24]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		mulsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm1, qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		subsd	xmm0, xmm1
		movsd	qword [buffer], xmm0
		push	qword [buffer]
		pop		qword [buffer]
		movsd	xmm0, qword [buffer]
		jmp		.ret_CalcDiscr
.ret_CalcDiscr:
		leave
		ret
r
.ret_CalcDiscr:
		leave
		ret
