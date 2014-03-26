#!/usr/bin/env sh
./gen_unrolled.py
parallel 'gsed -e "s/ROUNDS/{}/g" chacha-ref.c > chacha{}.c' ::: 8 12 20
