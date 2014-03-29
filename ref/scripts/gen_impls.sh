#!/usr/bin/env sh
cd ref
./scripts/gen_partially_unrolled.py
parallel 'gsed -e "s/ROUNDS/{}/g" chacha-ref-template.c > chacha{}.c' ::: 8 12 20
clang-format-3.5 -i -style=Chromium *.c *.h
