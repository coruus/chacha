#!/usr/bin/env python
"""Generate a partially unrolled ChaCha implementation."""
from __future__ import division, print_function

from numpy import uint8, uint32

_SIGMA = uint8([ord(c) for c in "expand 32-byte k"]).view(uint32)
SIGMA = ['{:#010}'.format(s) for s in _SIGMA]

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


def quarterround(a, b, c, d):
    """A ChaCha quarterround."""
    return (QUARTERROUND
            .replace("a", a)
            .replace("b", b)
            .replace("c", c)
            .replace("d", d))


DOUBLEROUND_MATRIX = [[0, 4, 8, 12],
                      [1, 5, 9, 13],
                      [2, 6, 10, 14],
                      [3, 7, 11, 15],
                      [0, 5, 10, 15],
                      [1, 6, 11, 12],
                      [2, 7, 8, 13],
                      [3, 4, 9, 14]]

DOUBLEROUND = [quarterround(*['x[{}]'.format(i) for i in r])
               for r in DOUBLEROUND_MATRIX]


_TEMPLATE = """\
static inline void _do_chacha{ROUNDS}(register uint32_t x[16],
                                      register const uint32_t input[16]);
static inline void _do_chacha{ROUNDS}(register uint32_t x[16],
                                      register const uint32_t input[16]) {{
  for (size_t i = 0; i < {doublerounds}; i++) {{
{DOUBLEROUND}
  }}
  {addinput}
}}
"""
_ADDINPUT = "  x[{i}] += input[{i}];"

ADDINPUT = '\n'.join(_ADDINPUT.format(i=i) for i in range(16))

for rounds in [8, 12, 20]:
    with open("chacha{}.gen.c".format(rounds), "wb") as f:
        f.write(_TEMPLATE.format(doublerounds=rounds//2,
                                 DOUBLEROUND='\n'.join(DOUBLEROUND),
                                 addinput=ADDINPUT,
                                 ROUNDS=rounds))
