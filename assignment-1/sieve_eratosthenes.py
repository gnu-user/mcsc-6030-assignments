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

print "List of primes: ", sieve(30)
