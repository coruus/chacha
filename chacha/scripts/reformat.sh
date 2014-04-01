#!/usr/bin/env bash
find . -iname "*.c" -or -iname "*.h" | parallel -j4 clang-format-3.5 -i -style=Chromium
