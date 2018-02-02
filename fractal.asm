





section .text
global _fractal

_fractal:

    push    rbp                 ;get ready stack
    mov     rbp, rsp            ;xd

    ;xmm0 = originR
    ;   1 = originI
    ;   2 = cR
    ;   3 = cI
    ;   14 = buffer
    ;   15 = scale
    ;   r15d = maxIt
    movsd   xmm14,  qword [rbp+56]  ;buffer
    movsd   xmm15,  qword [rbp+48]  ;scale
    mov     r15,    qword [rbp+64]  ;maxIt
    mov     r14,    qword [rbp+72]  ;*output

    mov     rax, 4                  ;load 4 to rax
    cvtsi2sd xmm9, rax              ;load to xmm9 to compare later

    mov     rax,    0               ;i for loop
    mov     rbx,    0               ;j for loop
    mov     rcx,    0               ;it
    cvtsd2si rdx,  xmm14            ;integer buffer to rdx

    movsd   xmm13, xmm15            ;13 = scale
    addsd   xmm13, xmm13            ;2*scale
    divsd   xmm14, xmm13            ;buffer/2*scale

fori:

    mov     rbx, 0                  ; j = 0
forj:
    cvtsi2sd    xmm10, rax          ; xmm10 = i
    divsd       xmm10, xmm14        ; i/(buffer/2*scale)
    subsd       xmm10, xmm15        ; xmm10 - scale
    addsd       xmm10, xmm0         ; zR

    cvtsi2sd    xmm12, rbx          ; xmm12 = j
    divsd       xmm12, xmm14        ; j/(buffer/2*scale)
    movsd       xmm11, xmm15        ; xmm11 = scale
    subsd       xmm11, xmm12        ; scale - xmm12
    addsd       xmm11, xmm1         ; zI

    mov         rcx,   0            ; it = 0

whileIt:
    movsd       xmm13, xmm10        ;temp zR
    mulsd       xmm10, xmm10        ;zR*zR
    movsd       xmm12, xmm11        ;zI
    mulsd       xmm12, xmm12        ;zI^2
    subsd       xmm10, xmm12        ;zR^2 - zI^2
    addsd       xmm10, xmm2         ;newZR = to co wyzej + cR

    mulsd       xmm11, xmm13        ;zR*zI
    addsd       xmm11, xmm11        ;2*zR*zI
    addsd       xmm11, xmm3         ;newZI = to co wyzej + cI

    movsd       xmm12, xmm10        ; zR
    movsd       xmm13, xmm11        ; zI
    mulsd       xmm12, xmm12        ; zR^2
    mulsd       xmm13, xmm13        ; zI^2
    addsd       xmm12, xmm13        ; zR^2 + zI^2
    ucomisd     xmm12, xmm9         ; wyzej compare to 4

    ja          endIt                ; break if wieksze xmm12 niz xmm9

    inc         rcx                 ; it++
    cmp         rcx, r15            ; it compare maxIt
    jl          whileIt

endIt:

    mov         dword[r14], ecx          ; output[i*buffer + j] = it
    add         r14, 4

    inc         rbx
    cmp         rbx, rdx
    jl          forj

endforj:

    inc         rax
    cmp         rax, rdx
    jl          fori
endfori:

    mov     rsp, rbp
    pop     rbp
    ret
