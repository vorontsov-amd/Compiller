global _start

extern printf

section .data
buffer: dq 0.0
str: db '%lg ', 0x0
section .rodata
const_0: dq 3.000000
const_1: dq 3.300000


section .text
_start: 
        call dtoa


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




dtoa:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 80
        mov     QWORD  [rbp-72], rdi
        movsd   QWORD  [rbp-80], xmm0
        mov     DWORD  [rbp-28], 0
        mov     rax, QWORD  [rbp-80]
        movq    xmm0, rax
        call    isNan
        test    eax, eax
        je      .L28
        mov     rax, QWORD  [rbp-72]
        mov     BYTE  [rax], 110
        mov     rax, QWORD  [rbp-72]
        add     rax, 1
        mov     BYTE  [rax], 97
        mov     rax, QWORD  [rbp-72]
        add     rax, 2
        mov     BYTE  [rax], 110
        mov     rax, QWORD  [rbp-72]
        add     rax, 4
        mov     BYTE  [rax], 0
        jmp     .L29
.L28:
        mov     rax, QWORD  [rbp-80]
        movq    xmm0, rax
        call    isInf
        test    eax, eax
        je      .L30
        mov     rax, QWORD  [rbp-72]
        mov     BYTE  [rax], 105
        mov     rax, QWORD  [rbp-72]
        add     rax, 1
        mov     BYTE  [rax], 110
        mov     rax, QWORD  [rbp-72]
        add     rax, 2
        mov     BYTE  [rax], 102
        mov     rax, QWORD  [rbp-72]
        add     rax, 4
        mov     BYTE  [rax], 0
        jmp     .L29
.L30:
        pxor    xmm0, xmm0
        ucomisd xmm0,   [rbp-80]
        jp      .L31
        pxor    xmm0, xmm0
        ucomisd xmm0,   [rbp-80]
        jne     .L31
        mov     rax, QWORD  [rbp-72]
        mov     BYTE  [rax], 48
        mov     rax, QWORD  [rbp-72]
        add     rax, 1
        mov     BYTE  [rax], 0
        jmp     .L29
.L31:
        mov     rax, QWORD  [rbp-72]
        mov     QWORD  [rbp-16], rax
        pxor    xmm0, xmm0
        comisd  xmm0,   [rbp-80]
        seta    al
        movzx   eax, al
        mov     DWORD  [rbp-32], eax
        cmp     DWORD  [rbp-32], 0
        je      .L33
        movsd   xmm0, QWORD  [rbp-80]

        fldz
        fld1
        fsubp
        fstp    qword [rbp-40]

        movsd    xmm1, QWORD  [rbp-40]
        mulsd   xmm0, xmm1
        movsd   QWORD  [rbp-80], xmm0
.L33:
        mov     rax, QWORD  [rbp-80]
        movq    xmm0, rax
        call    log10
        cvttsd2si       eax, xmm0
        mov     DWORD  [rbp-4], eax
        cmp     DWORD  [rbp-4], 13
        jg      .L34
        cmp     DWORD  [rbp-32], 0
        je      .L35
        cmp     DWORD  [rbp-4], 8
        jg      .L34
.L35:
        cmp     DWORD  [rbp-4], -8
        jge     .L36
.L34:
        mov     eax, 1
        jmp     .L37
.L36:
        mov     eax, 0
.L37:
        mov     DWORD  [rbp-36], eax
        cmp     DWORD  [rbp-32], 0
        je      .L38
        mov     rax, QWORD  [rbp-16]
        lea     rdx, [rax+1]
        mov     QWORD  [rbp-16], rdx
        mov     BYTE  [rax], 45
.L38:
        cmp     DWORD  [rbp-36], 0
        je      .L39
        cmp     DWORD  [rbp-4], 0
        jns     .L40
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, DWORD  [rbp-4]

        fld1
        fstp    qword [rbp-40]

        movsd   xmm1, QWORD  [rbp-40]
        subsd   xmm0, xmm1
        cvttsd2si       eax, xmm0
        mov     DWORD  [rbp-4], eax
.L40:
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, DWORD  [rbp-4]

        fld1
        fld1
        times 8 fadd    st1
        faddp st1, st0
        fstp    qword [rbp-40]

        mov     rax, QWORD  [rbp-40]
        movapd  xmm1, xmm0
        movq    xmm0, rax
        call    pow
        movapd  xmm1, xmm0
        movsd   xmm0, QWORD  [rbp-80]
        divsd   xmm0, xmm1
        movsd   QWORD  [rbp-80], xmm0
        mov     eax, DWORD  [rbp-4]
        mov     DWORD  [rbp-8], eax
        mov     DWORD  [rbp-4], 0
.L39:
        cmp     DWORD  [rbp-4], 0
        jg      .L42
        mov     DWORD  [rbp-4], 0
        jmp     .L42
.L47:
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, DWORD  [rbp-4]

        fld1
        fld1
        times 8 fadd    st1
        faddp st1, st0
        fstp    qword [rbp-64]


        mov     rax, QWORD  [rbp - 64]
        movapd  xmm1, xmm0
        movq    xmm0, rax
        call    pow
        movq    rax, xmm0
        mov     QWORD  [rbp-48], rax
        movsd   xmm0, QWORD  [rbp-48]
        pxor    xmm1, xmm1
        comisd  xmm0, xmm1
        jbe     .L43
        mov     rax, QWORD  [rbp-48]
        movq    xmm0, rax
        call    isInf
        test    eax, eax
        jne     .L43
        movsd   xmm0, QWORD  [rbp-80]
        divsd   xmm0,   [rbp-48]
        movq    rax, xmm0
        movq    xmm0, rax
        call    Floor
        cvttsd2si       eax, xmm0
        mov     DWORD  [rbp-52], eax
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, DWORD  [rbp-52]
        movapd  xmm1, xmm0
        mulsd   xmm1,   [rbp-48]
        movsd   xmm0, QWORD  [rbp-80]
        subsd   xmm0, xmm1
        movsd   QWORD  [rbp-80], xmm0
        mov     eax, DWORD  [rbp-52]
        lea     ecx, [rax+48]
        mov     rax, QWORD  [rbp-16]
        lea     rdx, [rax+1]
        mov     QWORD  [rbp-16], rdx
        mov     edx, ecx
        mov     BYTE  [rax], dl
.L43:
        cmp     DWORD  [rbp-4], 0
        jne     .L45
        movsd   xmm0, QWORD  [rbp-80]
        pxor    xmm1, xmm1
        comisd  xmm0, xmm1
        jbe     .L45
        mov     rax, QWORD  [rbp-16]
        lea     rdx, [rax+1]
        mov     QWORD  [rbp-16], rdx
        mov     BYTE  [rax], 46
.L45:
        sub     DWORD  [rbp-4], 1
.L42:
        movsd   xmm0, QWORD  [rbp-80]

        fld1
        fld1
        fld1
        times 8 fadd    st1
        faddp st1, st0
        fld st0
        times 14 fmul st1
        fmulp st1, st0
        fdivp st1, st0
        fstp    qword [rbp-56]

        comisd  xmm0,   [rbp-56]
        ja      .L47
        cmp     DWORD  [rbp-4], 0
        jns     .L47
        cmp     DWORD  [rbp-36], 0
        je      .L48
        mov     rax, QWORD  [rbp-16]
        lea     rdx, [rax+1]
        mov     QWORD  [rbp-16], rdx
        mov     BYTE  [rax], 101
        cmp     DWORD  [rbp-8], 0
        jle     .L49
        mov     rax, QWORD  [rbp-16]
        lea     rdx, [rax+1]
        mov     QWORD  [rbp-16], rdx
        mov     BYTE  [rax], 43
        jmp     .L50
.L49:
        mov     rax, QWORD  [rbp-16]
        lea     rdx, [rax+1]
        mov     QWORD  [rbp-16], rdx
        mov     BYTE  [rax], 45
        neg     DWORD  [rbp-8]
.L50:
        mov     DWORD  [rbp-4], 0
        jmp     .L51
.L52:
        mov     ecx, DWORD  [rbp-8]
        movsx   rax, ecx
        imul    rax, rax, 1717986919
        shr     rax, 32
        mov     edx, eax
        sar     edx, 2
        mov     eax, ecx
        sar     eax, 31
        sub     edx, eax
        mov     eax, edx
        sal     eax, 2
        add     eax, edx
        add     eax, eax
        sub     ecx, eax
        mov     edx, ecx
        mov     eax, edx
        lea     ecx, [rax+48]
        mov     rax, QWORD  [rbp-16]
        lea     rdx, [rax+1]
        mov     QWORD  [rbp-16], rdx
        mov     edx, ecx
        mov     BYTE  [rax], dl
        mov     eax, DWORD  [rbp-8]
        movsx   rdx, eax
        imul    rdx, rdx, 1717986919
        shr     rdx, 32
        mov     ecx, edx
        sar     ecx, 2
        cdq
        mov     eax, ecx
        sub     eax, edx
        mov     DWORD  [rbp-8], eax
        add     DWORD  [rbp-4], 1
.L51:
        cmp     DWORD  [rbp-8], 0
        jg      .L52
        mov     eax, DWORD  [rbp-4]
        cdqe
        neg     rax
        add     QWORD  [rbp-16], rax
        mov     DWORD  [rbp-20], 0
        mov     eax, DWORD  [rbp-4]
        sub     eax, 1
        mov     DWORD  [rbp-24], eax
        jmp     .L53
.L54:
        mov     eax, DWORD  [rbp-20]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        movzx   esi, BYTE  [rax]
        mov     eax, DWORD  [rbp-24]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        movzx   ecx, BYTE  [rax]
        mov     eax, DWORD  [rbp-20]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        xor     esi, ecx
        mov     edx, esi
        mov     BYTE  [rax], dl
        mov     eax, DWORD  [rbp-24]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        movzx   esi, BYTE  [rax]
        mov     eax, DWORD  [rbp-20]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        movzx   ecx, BYTE  [rax]
        mov     eax, DWORD  [rbp-24]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        xor     esi, ecx
        mov     edx, esi
        mov     BYTE  [rax], dl
        mov     eax, DWORD  [rbp-20]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        movzx   esi, BYTE  [rax]
        mov     eax, DWORD  [rbp-24]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        movzx   ecx, BYTE  [rax]
        mov     eax, DWORD  [rbp-20]
        movsx   rdx, eax
        mov     rax, QWORD  [rbp-16]
        add     rax, rdx
        xor     esi, ecx
        mov     edx, esi
        mov     BYTE  [rax], dl
        add     DWORD  [rbp-20], 1
        sub     DWORD  [rbp-24], 1
.L53:
        mov     eax, DWORD  [rbp-20]
        cmp     eax, DWORD  [rbp-24]
        jl      .L54
        mov     eax, DWORD  [rbp-4]
        cdqe
        add     QWORD  [rbp-16], rax
.L48:
        mov     rax, QWORD  [rbp-16]
        mov     BYTE  [rax], 0
.L29:
        mov     rax, QWORD  [rbp-72]
        mov     rdi, rax
        call    Strlen
        mov     DWORD  [rbp-28], eax
        mov     eax, DWORD  [rbp-28]
        leave
        ret