#!/usr/bin/env python
from __future__ import division, print_function

from functools import partial

from numpy import uint8, uint32

sigma = [ord(c) for c in "expand 32-byte k"]
sigma = uint8(sigma).view(uint32)
SIGMA = ['{:#010}'.format(s) for s in sigma]

QUARTERROUND = """\
  a += b;
  d ^= a;
  d = rol(d, 16);
  c += d;
  b ^= c;
  b = rol(b, 12);
  a += b;
  d ^= a;
  d = rol(d, 8);
  c += d;
  b ^= c;
  b = rol(b, 7);"""


def qround(a, b, c, d):
    """A ChaCha quarterround."""
    return (QUARTERROUND
            .replace("a", a)
            .replace("b", b)
            .replace("c", c)
            .replace("d", d))

def subs(a, b, c, d):
    return [x.replace('x', 'x[') + ']'
            for x in [a, b, c, d]]
DRMAT = [[0, 4,  8, 12],
         [1, 5,  9, 13],
         [2, 6, 10, 14],
         [3, 7, 11, 15],

         [0, 5, 10, 15],
         [1, 6, 11, 12],
         [2, 7,  8, 13],
         [3, 4,  9, 14]]

DROUND = [qround(*['x[{}]'.format(i) for i in r])
          for r in DRMAT]

#DROUND = [qround(*subs(*s.split(',')))
#          for s in
#          ['x0,x4,x8,x12',
#           'x1,x5,x9,x13',
#           'x2,x6,x10,x14',
#           'x3,x7,x11,x15',
#           'x0,x5,x10,x15',
#           'x1,x6,x11,x12',
#           'x2,x7,x8,x13',
#           'x3,x4,x9,x14']]

IS_CONS = "\n".join("  x[{i}] = {k};"
                    .format(i=i, k=sigma[i]) for i in range(4))
IS_PART = "  x[{si}] = {name}[{i}];"
IS_KEY = "\n".join(IS_PART.format(si=4 + i, i=i, name="key")
                   for i in range(8))
IS_NONCE = "\n".join(IS_PART.format(si=14 + i, i=i, name="nonce")
                     for i in range(2))
INIT_STATE = IS_CONS

for rounds in [8, 12, 20]:
    ROUNDS = DROUND * (rounds // 2)

    with open("chacha{}.gen.c".format(rounds), "wb") as f:
        print = partial(print, file=f)
        print('\n'.join(qr for qr in ROUNDS))
        PLUS = '\n'.join("  x[{i}] += input[{i}];"
                         .format(i=i) for i in range(16))
        print(PLUS)
