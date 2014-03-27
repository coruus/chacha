#!/usr/bin/env python
"""Generate the three ChaCha strengths."""

_TEMPLATE = """\
#define CHACHA_RNDS {rounds}
#define crypto_stream_xor chacha{rounds}_xor
#include "config.h"

#ifdef CHACHA_KROVETZ
#include "krovetz/chacha-krovetz.c"
#else
#include "ref/chacha-ref.c"
#endif  // CHACHA_KROVETZ"""

for rounds in [8, 12, 20]:
    with open('chacha{}.c'.format(rounds), 'wb') as f:
        f.write(_TEMPLATE.format(rounds=rounds))
