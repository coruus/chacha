#!/usr/bin/env python
"""Convert test vectors to cinits."""
from __future__ import division, print_function

from functools import partial

import parse

with open("chacha_testvectors.txt") as f:
    lines = f.readlines()

_PARSE_EXPRS = [
    ('keylen', 'Keylen: {}'),
    ('key', 'Key:    {}'),
    ('iv', 'IV:     {}'),
    ('rounds', 'Rounds: {}'),
    ('ks0', 'Keystream block 0: {}'),
    ('ks1', 'Keystream block 1: {}')]
_PARSES = [(name, parse.compile(expr)) for name, expr in _PARSE_EXPRS]
_LENS = {'ks0': 64, 'ks1': 64, 'iv': 8}

f = open("chacha-strombergson-kats.h", 'wb')

print = partial(print, file=f)

for line in lines:
    line = line.strip()
    if line.startswith('TC'):
        print("\n//" + '-' * 78)
        print("// {}".format(line))
        print("//" + '-' * 78)
        TC = int(line[2])
    else:
        parse = [(name, p.parse(line)[0])
                 for name, p in _PARSES
                 if p.parse(line)]
        if not parse:
            continue
        parse = parse[0]
        name = parse[0]
        if name == 'keylen':
            keylen = int(parse[1]) // 8
            continue
        elif name == 'rounds':
            rounds = parse[1]
            print()
            continue
        if name == 'key':
            length = keylen
        else:
            length = _LENS[name]
        print("static const uint8_t tc{TC}_chacha{rounds}"
              "_keylen{keylenbits}_{name}[{len}]"
              " = {{ {value} }};"
              .format(TC=TC, rounds=rounds, keylenbits=keylen * 8,
                      keylen=keylen, len=length, name=name,
                      value=parse[1].replace(' ', ', ')))
f.close()
