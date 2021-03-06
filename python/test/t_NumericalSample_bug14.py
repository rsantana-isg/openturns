#! /usr/bin/env python

from __future__ import print_function
from openturns import *

TESTPREAMBLE()

try:
    sample1 = NumericalSample(5, NumericalPoint(1, 2.0))
    sample2 = NumericalSample(sample1)
    sample2[0] = NumericalPoint(1, 3.0)
    print("sample1=", repr(sample1))
    print("sample2=", repr(sample2))
    sample1[1] = NumericalPoint(1, -5.0)
    print("sample1=", repr(sample1))
    print("sample2=", repr(sample2))
    sample1[3, 0] = 6.0
    print("sample1=", repr(sample1))
    print("sample1[2][0]=", sample1[2][0])
    print("sample1[2, 0]=", sample1[2, 0])
    try:
        sample1[1, 2] = -4.0
    except:
        print("set item failed on purpose")

except:
    import sys
    print("t_NumericalSample_bug14.py", sys.exc_info()[0], sys.exc_info()[1])
