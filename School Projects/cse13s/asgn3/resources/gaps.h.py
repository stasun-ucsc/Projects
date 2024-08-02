#!/usr/bin/env python3
#
# NAME
#       gaps.h.py - Generate a gap sequence for Shell Sort
#
# SYNOPSIS
#       ./gaps.h.py [MAX]
#
# DESCRIPTION
#       This script will generate a C header file which contains the
#       Pratt Sequence (2^p * 3^q also called 3-smooth) stored as a
#       global array: gaps.  It will also define the GAPS macro being
#       the total number of gaps in the sequence.
#
#       You may specify the maximum gap size for the sequence, or
#       allow it to default to 1,000,000.
#
# EXAMPLES
#       ./gaps.h.py > gaps.h
#       ./gaps.h.py 10 > gaps.h
#
from sys import argv
from os import system


def main():
    MAX = 1000000
    if len(argv) == 1: pass
    elif len(argv) == 2 and argv[1].isnumeric():
        MAX = int(argv[1])
    else:
        system(f"grep '^#' {argv[0]}")
        return
    pratt = sorted(genPratt(MAX), reverse=True)

    declaration = 'static uint32_t const gaps[] = {'
    pad = ' ' * (len(declaration) - 1)
    end = f'{pad}' + '};'

    print('#pragma once')
    print('#include <stdint.h>')
    print(f'#define GAPS {len(pratt)}')

    print(declaration, end='')
    for gap in pratt:
        print(f' {gap}\n{pad},', end='')
    print(f'\n{end}')


def genPratt(max: int, seq: set = set(), p: int = 1, q: int = 1):
    gap = p * q
    if gap <= max:
        seq.add(gap)
        genPratt(max, seq, p * 2, q)
        genPratt(max, seq, p , q * 3)
    return seq


if __name__ == '__main__':
    main()
