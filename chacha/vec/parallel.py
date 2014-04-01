from __future__ import division, print_function


STATIC_DOC = """\
t   xmm0

x0  xmm1
x4  xmm2
x8  xmm3
x12 xmm4

x1  xmm5
x5  xmm6
x9  LS
x13 xmm7

x2  xmm8
x6  xmm9
x10 xmm10
x14 xmm11

x3  xmm12
x7  xmm13
x11 LS
x15 xmm14
"""

STATIC_REGS = \
    [(0, 1),
     (4, 2),
     (8, 3),
     (12, 4),
     (1, 5),
     (5, 6),
     (13, 7),
     (2, 8),
     (6, 9),
     (10, 10),
     (14, 11),
     (3, 12),
     (7, 13),
     (15, 14)]
MOV = "movdqa   x{i}, [x+{offset}]"
REGDEF = "%define x{i} xmm{xmm}"

for i, xmm in STATIC_REGS:
    print(REGDEF.format(i=i, xmm=xmm))
    #print(MOV.format(offset=i*16, i=i))


QUARTER_ROUND = """\
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
"""

SWAP119 = """\
  movdqa  stack11, xmm15 ; store x11
  movdqa  xmm15, stack9  ; load x9"""
SWAP911 = """\
  movdqa  stack9, xmm15  ; store x9
  movdqa  xmm15, stack11 ; load x11"""

def qr(s):
    return "  quarterround {}, {}, {}, {}".format(*s.split(','))

DR = [qr("x0,x4,x8,x12"),
      qr("x1,x5,x9,x13"),
      SWAP911,
      qr("x2,x6,x10,x14"),
      qr("x3,x7,x11,x15"),
      qr("x0,x5,x10,x15"),
      qr("x1,x6,x11,x12"),
      SWAP119,
      qr("x2,x7,x8,x13"),
      qr("x3,x4,x8,x14")]

print('\n'.join(DR))

PADD = """\
  paddw   x{i}, input({i})
  movdqa  input(i), x{i}"""

PADD = "  addi x{i}, input({i})"

def padd(i):
    return PADD.format(i=i)

ADDI = '\n'.join([padd(i) for i in range(0, 11)]
                 + ['   movdqa x11, stack11 ; restore x11']
                 + [padd(i) for i in range(11, 16)])
print(ADDI)
