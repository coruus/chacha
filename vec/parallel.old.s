

%macro quarterround
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

initialization:
movdqa  xmm15, input(9)
movdqa  stack11, input(11)


swap911:
movdqa  stack9, xmm15
movdqa  xmm15, stack11

swap119:


x0, x4, x8,  x12
x1, x5, x9,  x13    store x9  load x11
x2, x6, x10, x14
x3, x7, x11, x15

x0, x5, x10, x15
x1, x6, x11, x12    store x11 load x9
x2, x7, x8,  x13
x3, x4, x9,  x14


t   xmm0
x0  xmm1
x4  xmm2
x8  xmm3
x12 xmm4
x1  xmm5
x5  xmm6
x9  store x7 to X7, load X9 to xmm7
x13 xmm8
x2  xmm9
x6  xmm10
x10 xmm11
x14 xmm12
x3  xmm13
x7  store x9 to X9, load X7 to xmm7
x11 xmm14
x15 xmm15



