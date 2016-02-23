#!/usr/bin/env python

import os
import sys

assert(len(sys.argv) == 2)

MARGIN = int(sys.argv[1])
assert(MARGIN >= 0)

VALS = []
for aline in sys.stdin:
    aline = aline.strip()
    if (aline == ""):
        continue
    VALS.append(float(aline)) 

assert(2*MARGIN < len(VALS))

VALS.sort()

VALS = VALS[ MARGIN : len(VALS)-MARGIN ]

print (str((sum(VALS) / float(len(VALS)))))
