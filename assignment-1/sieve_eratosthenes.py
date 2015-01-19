#!/usr/bin/env python2
###############################################################################
#
# Sieve of Eratosthenes Implementation.
#
# Implementation based on the optimized implementation pseudocode provided on
# Wikipedia: http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
# Copyright (C) 2014, Jonathan Gillett (100437638)
# All rights reserved.
#
###############################################################################
from math import ceil, sqrt
import sys
from time import time
import getopt


def sieve(n):
    """Returns the list of primes using the Sieve of Eratosthenes"""
    A = [True] * (n-1)

    for i in xrange(2, int(ceil(sqrt(n)))+1):
        if A[i-2]:
            for k in xrange(0, n+1):
                j = i**2 + k*i
                if j > n:
                    break
                A[j-2] = False

    return [i+2 for i in xrange(0, len(A)) if A[i]]

# Get the n provided if applicable
n = None
print_primes = True
args = sys.argv

# Hack to get the first argument as N, if provided
if len(args) > 1 and args[1].isdigit():
    n = int(sys.argv[1])
    args = args[2:]

try:
    opts, args = getopt.getopt(args, '', ['print='])
except getopt.GetoptError:
    print 'sieve_eratosthenes.py --print=[true,false]'
    sys.exit(2)

for opt, arg in opts:
    if opt in ("--print"):
        if arg.lower() == 'false':
            print_primes = False

# Calculate the number of primes provided or best with time limit
if n is not None:
    primes = sieve(n)
    if print_primes:
        print "List of primes:", primes
else:
    # Continue multiplying the number of primes until it takes about 60 seconds
    n = 1000
    run_time = 0
    primes = []

    while run_time < 60:
        start_time = time()
        primes = sieve(n)
        run_time = time() - start_time
        n *= 10

    print "Solved primes using Sieve of Eratosthenes up to %d in %.2f seconds!" % (n, run_time)
    if print_primes:
        print "List of primes:", primes
