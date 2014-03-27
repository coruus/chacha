#!/usr/bin/env sh
./scripts/gen_unrolled.py
parallel 'gsed -e "s/ROUNDS/{}/g" chacha-ref-stub.c > chacha{}.c' ::: 8 12 20
