#!/usr/bin/env sh
cf="$(which clang-format||
      which clang-format-3.5||
      which clang-format-HEAD||
      which clang-format-3.4||
      [-f /usr/local/bin/clang-format&&printf /usr/local/bin/clang-format||
      printf clang-format)"
parallel="(which parallel||
           [-f /usr/local/bin/
find . -name "*.c" -o -name "*.h" | parallel "${cf} -i -style=file {}"
