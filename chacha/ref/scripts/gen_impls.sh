#!/usr/bin/env sh
cd ref
./scripts/gen_partially_unrolled.py
parallel 'gsed -e "s/ROUNDS/{}/g" chacha.template.c > gen/chacha{}.c' ::: 8 12 20
cp -f chachas.template.c chachas.c
clang-format-3.5 -i -style=file *.c *.h
