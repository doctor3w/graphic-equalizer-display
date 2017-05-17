#! /bin/bash
find . \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} \;
