#!/usr/bin/env python
"""Generate test-cases for 256-bit keylength."""
from __future__ import division, print_function

from functools import partial

_TEMPLATE = """\
ret += check_case({p}_key,
                  {p}_iv,
                  {p}_ks0,
                  {p}_ks1,
                  {rounds},
                  {name});"""
_P = """tc{tcnum}_chacha{rounds}_keylen256"""
_NAME = '"{tc} - {p}"'

_TCS = [(1, 'TC1: All zero key and IV.'),
        (2, 'TC2: Single bit in key set. All zero IV.'),
        (3, 'TC3: Single bit in IV set. All zero key.'),
        (4, 'TC4: All bits in key and IV are set.'),
        (5, 'TC5: Every even bit set in key and IV.'),
        (6, 'TC6: Every odd bit set in key and IV.'),
        (7, 'TC7: Sequence patterns in key and IV.'),
        (8, "TC8: key: 'All your base are belong to us!, IV: 'IETF2013'")]

with open('strombergson-cases.gen.c', 'wb') as f:
    print = partial(print, file=f)
    for tcnum, tcname in _TCS:
        print("\n//" + '-' * 78)
        print("// {}".format(tcname))
        print("//" + '-' * 78)
        for rounds in [8, 12, 20]:
            p = _P.format(tcnum=tcnum, rounds=rounds)
            name = _NAME.format(tc=tcname, p=p)
            print(_TEMPLATE.format(p=p, name=name, rounds=rounds))
