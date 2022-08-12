global _start
section .text
_start: 
        call atod


pow:
        push    rbp
        mov     rbp, rsp
        movsd   qword [rbp - 8], xmm1
        fld     qword [rbp - 8]
        movsd   qword [rbp - 8], xmm0
        fld     qword [rbp - 8]
        fyl2x
        fld     st0
        frndint
        fsubr   st0, st1
        f2xm1 
        fld1
        faddp 
        fscale
        fstp    qword [rbp - 8]
        movsd   xmm0, qword [rbp - 8]
        fstp    qword [rbp - 8]
        pop     rbp
        ret


log10:
        push    rbp
        mov     rbp, rsp
        movsd   qword [rbp-8], xmm0
        fld1
        fld     qword [rbp-8]
        fyl2x
        fldl2t
        fdivp   st1, st0
        fstp    qword [rbp-8]
        movsd   xmm0, qword [rbp-8]
        pop     rbp
        ret





dtoa:
%define dtoa_str [rbp-8] 
%define dtoa_num [rbp-16] 
%define dtoa_buffer [rbp-24]
%define dtoa_exp [rbp-32]
%define dtoa_e [rbp-42]
%define dtoa_digit [rbp-48]
                push    rbp
                mov     rbp, rsp
                sub     rsp, 48
                mov     qword dtoa_str, rdi
                movsd   qword dtoa_num, xmm0
                call    isNan
                test    rax, rax
                mov     rsi, qword dtoa_str
                je      .if_inf
.if_nan:        mov     byte [rsi], 'n'
                inc     rsi
                mov     byte [rsi], 'a'
                inc     rsi 
                mov     byte [rsi], 'n'
                inc     rsi
                mov     byte [rsi], 0
                jmp     .end_if

.if_inf:        call    isInf
                test    rax, rax
                je      .if_zero
                mov     byte [rsi], 'i'
                inc     rsi
                mov     byte [rsi], 'n'
                inc     rsi 
                mov     byte [rsi], 'f'
                inc     rsi
                mov     byte [rsi], 0
                jmp     .end_if 

.if_zero:       mov     rax, qword dtoa_num
                test    rax, rax
                jne     .if_else
                mov     byte [rsi], '0'
                inc     rsi
                mov     byte [rsi], 0
                jmp     .end_if

.if_else:       movsd   xmm0, qword dtoa_num
                mov     rax, 0
                movq    xmm1, rax
                comisd  xmm0, xmm1
.if_num_less_zero: 
                jae     .if_num_less_zero_end
                mov     rax, -1
                cvtsi2sd xmm1, rax
                mulsd   xmm0, xmm1
                movsd   qword dtoa_num, xmm0
                mov     byte [rsi], '-'
                inc     rsi
                
.if_num_less_zero_end:         
                movsd   xmm0, qword dtoa_num
                call    log10
                call    Floor
                cvttsd2si rax, xmm0
                mov     qword dtoa_exp, rax

                mov     rdx, 13
                cmp     rax, rdx
                jg      .if_use_exp

                mov     rdx, -9
                cmp     rax, rdx
                jge     .if_use_exp_end

.if_use_exp:    
                mov     rax, 10
                cvtsi2sd xmm0, rax
                cvtsi2sd xmm1, qword dtoa_exp
                call    pow
                movsd   xmm1, qword dtoa_num
                divsd   xmm1, xmm0

                movsd   qword dtoa_num, xmm1
                cvttsd2si rdx, xmm1
                add     rdx, '0'
                mov     byte [rsi], dl
                inc     rsi
                mov     byte [rsi], '.'
                inc     rsi
                mov     ecx, 6

.loop0:         
                movsd   xmm0, qword dtoa_num
                cvttsd2si rdx, xmm0
                cvtsi2sd xmm1, rdx
                subsd   xmm0, xmm1
                mov     rax, 10
                cvtsi2sd xmm1, rax
                mulsd   xmm0, xmm1
                movsd   qword dtoa_num, xmm0 
                cvttsd2si rdx, xmm0
                add     rdx, '0'
                mov     byte [rsi], dl
                inc     rsi
                loop    .loop0
                
                mov     byte [rsi], 'e'
                inc     rsi

                mov     rax, dtoa_exp
                mov     rdx, 0
                cmp     rax, rdx
                jge     .if_exp_less_zero_end
.if_exp_less_zero:
                                
                mov     rdx, -1
                imul     rax, rdx

                mov     qword dtoa_exp, rax
                mov     byte [rsi], '-'
                inc     rsi
.if_exp_less_zero_end:
                
                cvtsi2sd xmm0, rax
                call    log10
                cvttsd2si rax, xmm0
                mov     qword dtoa_e, rax

                mov     rdi, qword dtoa_exp
                mov     rdx, rsi
                mov     rsi, qword dtoa_e
                call    itoa
                mov     rsi, rax
                jmp     .end_if

.if_use_exp_end:
                movsd   xmm1, qword dtoa_num
                cvttsd2si rax, xmm1
                mov     qword dtoa_digit, rax
                cvtsi2sd xmm0, rax
                subsd   xmm1, xmm0
                movsd   qword dtoa_num, xmm1
                mov     rdi, rax
                mov     rdx, rsi
                mov     rsi, qword dtoa_exp
                call    itoa
                mov     rsi, rax

                mov     edx, 1
                mov     edi, 10000000
                cvtsi2sd xmm3, edi
                cvtsi2sd xmm2, edx
                mulsd   xmm3, xmm3
                divsd   xmm2, xmm3
                movsd   xmm4, dtoa_num
                comisd  xmm4, xmm2
                jb      .end_if


                mov     byte [rsi], '.'
                inc     rsi

                mov     ecx, 15
                mov     ebx, 10

.loop1:         
                mov     eax, 10
                cvtsi2sd xmm0, eax
                mulsd   xmm0, dtoa_num
                cvttsd2si rax, xmm0
                cvtsi2sd xmm1, rax
                subsd   xmm0, xmm1
                movsd   qword dtoa_num, xmm0
                add     rax, '0'
                mov     byte [rsi], al
                inc     rsi

                cvtsi2sd xmm0, ebx
                mov     r8, 15
                sub     r8, rcx
                cvtsi2sd xmm1, r8
                call    pow
                mulsd   xmm0, xmm2
                movsd   xmm1, qword dtoa_num
                comisd  xmm1, xmm0
                jb      .end_if

                loop    .loop1

.end_if:
                mov     byte [rsi], 0
                mov     rdi, dtoa_str
                call    Strlen
                leave
                ret






isNan:
        push    rbp
        mov     rbp,  rsp
        movsd   QWORD [rbp-8], xmm0
        mov     eax,  1
        movsd   xmm0, QWORD [rbp-8]
        ucomisd xmm0, [rbp-8]
        jp      .L10
        mov     eax, 0
.L10:
        movzx   eax, al
        pop     rbp
        ret


isInf:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 8
        movsd   QWORD [rbp-8], xmm0
        mov     rax, QWORD [rbp-8]
        movq    xmm0, rax
        call    isNan
        test    eax, eax
        jne     .L13
        movsd   xmm0, QWORD [rbp-8]
        subsd   xmm0, xmm0
        movq    rax, xmm0
        movq    xmm0, rax
        call    isNan
        test    eax, eax
        je      .L13
        mov     eax, 1
        jmp     .L15
.L13:
        mov     eax, 0
.L15:
        leave
        ret

Strlen:
        push    rbp
        mov     rbp, rsp
        mov     QWORD [rbp-24], rdi
        mov     DWORD [rbp-4], 0
        jmp     .L6
.L7:
        add     DWORD [rbp-4], 1
        add     QWORD [rbp-24], 1
.L6:
        mov     rax, QWORD [rbp-24]
        movzx   eax, BYTE [rax]
        test    al, al
        jne     .L7
        mov     eax, DWORD [rbp-4]
        pop     rbp
        ret


itoa:
%define itoa_digit [rbp-8]
%define itoa_exp [rbp-16]
%define itoa_str [rbp-24]
%define itoa_c [rbp-32]
                push    rbp
                mov     rbp, rsp
                sub     rsp, 32
                mov     qword itoa_digit, rdi
                mov     qword itoa_exp, rsi
                mov     qword itoa_str, rdx
                mov     qword itoa_c, rdx
.while_loop:
                mov     rsi, itoa_str
                mov     rdi, qword itoa_digit
                mov     rsi, 10
                call    mod
                add     rax, '0'
                mov     rsi, itoa_str
                mov     byte [rsi], al
                inc     rsi
                mov     qword itoa_str, rsi
                mov     rdi, qword itoa_digit
                mov     rsi, 10
                call    divq       
                mov     qword itoa_digit, rax
                test    rax, rax         
                jne     .while_loop

                mov     rax, itoa_exp
                xor     rdx, rdx
                cmp     rax, rdx
                jle     .leave

                mov     r8, itoa_c
                lea     r9, [r8+rax]
                jmp     .test
.loop:
                mov     bl, byte [r8]
                mov     dl, byte [r9]
                mov     byte [r8], dl
                mov     byte [r9], bl
                inc     r8
                dec     r9
.test:          cmp     r8, r9
                jb      .loop

.leave:
                mov     rax, itoa_str
                leave
                ret



divq:
        push    rbp
        mov     rbp, rsp
        mov     QWORD [rbp-8], rdi
        mov     QWORD [rbp-16], rsi
        mov     rax, QWORD [rbp-8]
        cqo
        idiv    QWORD [rbp-16]
        pop     rbp
        ret
mod:
        push    rbp
        mov     rbp, rsp
        mov     QWORD [rbp-8], rdi
        mov     QWORD [rbp-16], rsi
        mov     rax, QWORD [rbp-8]
        cqo
        idiv    QWORD [rbp-16]
        mov     rax, rdx
        pop     rbp
        ret




Floor:
        push    rbp
        mov     rbp, rsp
        movsd   QWORD  [rbp-8], xmm0
        movsd   xmm0, QWORD  [rbp-8]
        pxor    xmm1, xmm1
        comisd  xmm0, xmm1
        jb      .L22
        movsd   xmm0, QWORD  [rbp-8]
        cvttsd2si       eax, xmm0
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, eax
        jmp     .L20
.L22:
        movsd   xmm0, QWORD  [rbp-8]

        fld1
        fstp qword [rbp-16]

        movsd   xmm1, QWORD  [rbp-16]
        subsd   xmm0, xmm1
        cvttsd2si       eax, xmm0
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, eax
.L20:
        movq    rax, xmm0
        movq    xmm0, rax
        pop     rbp
        ret


isDigit:
        push    rbp
        mov     rbp, rsp
        mov     eax, edi
        mov     BYTE [rbp-4], al
        cmp     BYTE [rbp-4], 47
        jle     .L24
        cmp     BYTE [rbp-4], 57
        jg      .L24
        mov     eax, 1
        jmp     .L26
.L24:
        mov     eax, 0
.L26:
        pop     rbp
        ret



atod:
%define atod_res [rbp-8]
%define atod_sign [rbp-16]
%define atod_exp [rbp-24]
%define atod_exp_sign [rbp-32]
%define atod_str [rbp-42]
        push    rbp
        mov     rbp, rsp
        sub     rsp, 48
        mov     qword atod_res, 0
        mov     qword atod_sign, 1
        mov     qword atod_str, rdi
        mov     rsi, rdi
        mov     al, byte [rsi]
        cmp     al, '-'
        jne     .sign_end
.sign_start:
        mov     qword atod_sign, -1
        inc     rsi
.sign_end:
        jmp     .while_0_test
.while_0_loop:
        mov     rax, 10
        cvtsi2sd xmm0, rax
        movsd   xmm1, qword atod_res
        mulsd   xmm1, xmm0
        xor     rax, rax
        mov     al, byte [rsi]
        inc     rsi
        sub     al, '0'
        cvtsi2sd xmm0, rax
        addsd   xmm1, xmm0
        movsd   qword atod_res, xmm1
.while_0_test:
        xor     rax, rax
        mov     al, byte [rsi] 
        mov     rdi, rax
        mov     qword atod_str, rsi
        call    isDigit
        mov     rsi, qword atod_str
        test    rax, rax
        jne     .while_0_loop  

        mov     al, byte [rsi]
        cmp     al, '.'
        jne     .not_integer
.is_integer:
        inc     rsi
        mov     qword atod_exp, 0
        jmp     .while_1_test

.while_1_loop:
        mov     rax, 10
        cvtsi2sd xmm0, rax
        movsd   xmm1, qword atod_res
        mulsd   xmm1, xmm0
        xor     rax, rax
        mov     al, byte [rsi]
        inc     rsi
        sub     al, '0'
        cvtsi2sd xmm0, rax
        addsd   xmm1, xmm0
        movsd   qword atod_res, xmm1
        dec     qword atod_exp      
.while_1_test:
        xor     rax, rax
        mov     al, byte [rsi] 
        mov     rdi, rax
        mov     qword atod_str, rsi
        call    isDigit
        mov     rsi, qword atod_str
        test    rax, rax
        jne     .while_1_loop

        mov     rax, 10
        cvtsi2sd xmm0, rax
        cvtsi2sd xmm1, qword atod_exp
        call    pow
        movsd   xmm1, qword atod_res
        mulsd   xmm1, xmm0
        movsd   qword atod_res, xmm1
.not_integer:
        mov     al, byte [rsi]
        cmp     al, 'e'
        jne     .exit
.is_exponent:
        inc     rsi
        mov     qword atod_str, rsi
        mov     rdi, rsi
        call    atod
        movapd  xmm1, xmm0
        mov     rax, 10
        cvtsi2sd xmm0, rax
        call    pow
        movsd   xmm1, qword atod_res
        mulsd   xmm1, xmm0
        movsd   qword atod_res, xmm1

.exit:  movsd   xmm0, atod_res
        cvtsi2sd xmm1, qword atod_sign
        mulsd   xmm0, xmm1
        leave
        ret


