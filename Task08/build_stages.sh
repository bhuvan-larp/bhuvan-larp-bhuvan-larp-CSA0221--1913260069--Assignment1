#!/bin/sh
# Task 8: each stage of compilation, then one example of each error class.
set -x
gcc -E task08.c -o task08.i          # 1 preprocess: includes expanded, macros replaced, comments removed
gcc -S task08.c -o task08.s          # 2 compile to assembly
gcc -c task08.c -o task08.o          # 3 assemble to object file
gcc task08.o -o task08               # 4 link to executable
./task08
wc -l task08.c task08.i task08.s
set +x
echo "--- compile-time error ---"; gcc error_compile.c -o /dev/null 2>&1 | head -8
echo "--- link-time error ---";    gcc error_link.c -o /dev/null 2>&1 | head -8
echo "--- run-time error ---";     gcc error_runtime.c -o error_runtime && ./error_runtime; echo "exit status: $?"
echo "--- warnings with -Wall -Wextra ---"; gcc -Wall -Wextra warning_demo.c -o /dev/null 2>&1 | head -12
