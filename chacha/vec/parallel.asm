[bits 64]
[CPU intelnop]

%define arg1 rdi
%define arg2 rsi
%define arg3 rdx
%define arg4 rcx
%define arg5 r8

%define outptr   arg1
%define in       arg2
%define key      arg3
%define nonce    arg4
%define position arg5

%macro quarterround 4
    %define a %1
    %define b %2
    %define c %3
    %define d %4
    ; a += b; d ^= a; t = d; t <<= 16; d >>= 16; d ^= t;
    paddw    a, b
    pxor     d, a
    movdqa   t, d
    psllw    t, 16
    psrlw    d, 16
    pxor     d, t
    ; c += d; b ^= c; t = b; t <<= 12; b >>= 20; b ^= t;
    paddw    c, d
    pxor     b, c
    movdqa   t, b
    psllw    t, 12
    psrlw    b, 20
    pxor     b, t
    ; a += b; d ^= a; t = d; t <<= 8;  d >>= 24; d ^= t;
    paddw    a, b
    pxor     d, a
    movdqa   t, d
    psllw    t, 8
    psrlw    d, 24
    pxor     d, t
    ; c += d; b ^= c; t = b; t <<= 7; b >>= 25; b ^= t;
    paddw    c, d
    pxor     b, c
    movdqa   t, b
    psllw    t, 7
    psrlw    b, 25
    pxor     b, t
%endmacro

%define t   xmm0
%define x9  xmm15
%define x11 xmm15

%define x0  xmm1
%define x4  xmm2
%define x8  xmm3
%define x12 xmm4
%define x1  xmm5
%define x5  xmm6
%define x13 xmm7
%define x2  xmm8
%define x6  xmm9
%define x10 xmm10
%define x14 xmm11
%define x3  xmm12
%define x7  xmm13
%define x15 xmm14

%define out(i) [outptr+i*16]
%macro addi 2
  paddw  %1, %2
  movdqa %2, %1
%endmacro

%define stack11 [stack0 wrt rip]
%define stack9 [stack1 wrt rip]
%define add0123 [_add0123 wrt rip]

%define k256_0 1634760805
%define k256_1 857760878
%define k256_2 2036477234
%define k256_3 1797285236

section .data
align 32

_add0123:
  dq 0
  dq 1
  dq 2
  dq 3

section .bss
align 32

stack0: resb 64
stack1: resb 64

%macro loadk32 2
  %define xmm %1
  %define const %2
  mov    r10, const
  movd   xmm, r10
  pshufd xmm, xmm, 0
%endmacro

section .text
align 32

global chacha4block
chacha4block:

push rbp
mov  rbp, rsp
;sub rsp, 4 * 4 * 2 ; two stack locs


broadcast_input:
  loadk32 x0,  k256_0
  loadk32 x1,  k256_1
  loadk32 x2,  k256_2
  loadk32 x3,  k256_3
load_key:
  pshufd  x4,  [key+0],  0
  pshufd  x5,  [key+4],  0
  pshufd  x6,  [key+8],  0
  pshufd  x7,  [key+12], 0
  pshufd  x8,  [key+16], 0
  pshufd  x9,  [key+20], 0
  movdqa  stack9, x9            ; store x9
  pshufd  x10, [key+24], 0
  pshufd  x11, [key+28], 0
  movdqa  stack11, x11          ; store x11
  movdqa  x9, stack9            ; restore x9
;load_pos:
;  movdqa  x12, [position+0]
;  movdqa  x13, [position+16]
;add_pos:
;  paddq   x13, [_add0123 wrt rip]
;  ;pshufd  x12, [position+0], 0
;  ;pshufd  x13, [position+16], 0
;load_nonce:
;  pshufd  x14, [nonce+0], 0
;  pshufd  x15, [nonce+16], 0

%macro doubleround 0
  quarterround x0, x4, x8, x12
  quarterround x1, x5, x9, x13
  movdqa  stack9, xmm15          ; store x9
  movdqa  xmm15, stack11         ; load x11
  quarterround x2, x6, x10, x14
  quarterround x3, x7, x11, x15
  quarterround x0, x5, x10, x15
  quarterround x1, x6, x11, x12
  movdqa  stack11, xmm15         ; store x11
  movdqa  xmm15, stack9          ; load x9
  quarterround x2, x7, x9, x13
  quarterround x3, x4, x8, x14
%endmacro

doubleround
doubleround
doubleround
doubleround
doubleround
doubleround

write_output:
  movdqu  out(0), x0
  movdqu  out(1), x1
  movdqu  out(2), x2
  movdqu  out(3), x3
  movdqu  out(4), x4
  movdqu  out(5), x5
  movdqu  out(6), x6
  movdqu  out(7), x7
  movdqu  out(8), x8
  movdqu  out(9), x9
  movdqa  x11, stack11
  movdqu  out(10), x10
  movdqu  out(11), x11
  movdqu  out(12), x12
  movdqu  out(13), x13
  movdqu  out(14), x14
  movdqu  out(15), x15

  pxor    x0, x0
  pxor    x1, x1
  movdqa  stack9, x0
  movdqa  stack11, x1
  pxor x2, x2
  pxor x3, x3
  pxor x4, x4
  pxor x5, x5
  pxor x6, x6
  pxor x7, x7
  pxor x8, x8
  pxor x9, x9
  pxor x10, x10
  ;pxor x11, x11
  pxor x12, x12
  pxor x13, x13
  pxor x14, x14
  pxor x15, x15

mov rbp, rsp
pop rbp

ret
