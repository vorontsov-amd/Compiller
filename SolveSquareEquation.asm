global _start
extern dtoa, atod, pow

section .data
buffer: dq 0.0
str: times 32 db 0
section .rodata
const_0: dq 1.000000
const_1: dq 1.000000
const_2: dq 1.000000
const_3: dq 1.000000
const_4: dq 1.000000
const_5: dq 1.000000
const_6: dq 1.000000
const_7: dq 2.000000
const_8: dq 1.000000
const_9: dq 2.000000
const_10: dq 1.000000
const_11: dq 1.000000
const_12: dq 2.000000
const_13: dq 0.000000
const_14: dq 0.000000
const_15: dq 1.000000
const_16: dq 0.000000
const_17: dq 0.000000
const_18: dq 0.000000
const_19: dq 0.000000
const_20: dq 0.000000
const_21: dq 0.000000
const_22: dq -1.000000
const_23: dq 0.000000
const_24: dq 1.000000
const_25: dq 0.000000
const_26: dq 0.000000
const_27: dq 0.000000
const_28: dq 1.000000
const_29: dq 2.000000
const_30: dq 2.000000
const_31: dq 2.000000
const_32: dq 2.000000
const_33: dq 4.000000
str_0: db 'enter'
str_0_len: equ $-str_0
str_1: db 'a'
str_1_len: equ $-str_1
str_2: db 'enter'
str_2_len: equ $-str_2
str_3: db 'b'
str_3_len: equ $-str_3
str_4: db 'enter'
str_4_len