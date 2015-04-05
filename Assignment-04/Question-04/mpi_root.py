#!/usr/bin/env python2
###############################################################################
#
# Assignment 4, Question 4 solution using MPI in Python.
#
# Copyright (C) 2014, Jonathan Gillett (100437638)
# All rights reserved.
#
###############################################################################
import numpy as np
from mpi4py import MPI

# Define process 0 as MASTER
MASTER = 0
# Set the subinterval limit
INTERVAL = 10E-9
# Set the width for search space
WIDTH = 1E6


def f(x):
    return -2.0 + np.sin(x) + np.sin(x**2) + np.sin(x**3) + \
        np.sin(x**999) + np.sin(x**1000)


def master(n_proc, comm):
    """The master process divides the interval [0, 1] into subintervals."""
    # Divide the interval [0,1] into several subintervals for each process
    intervals = np.linspace(0, 1, n_proc)
    ANS = np.zeros(2, dtype=np.float64)  # The result and r value from processes
    SOL = np.ones(2, dtype=np.float64)  # The final smallest solution and r value
    n_sent = 0  # Number of processes evaluating subintervals

    # Assign to each task an endpoint of one of the subintervals at which the
    # function f should be evaluated
    for k in range(1, n_proc):
        # Send the initial subintervals and width to each process
        comm.Send(intervals[k-1:k+1], k, tag=WIDTH)
        n_sent += 1

    while n_sent > 0:
        status = MPI.Status()
        # Receive the r value and result that closest approximates function f(x)
        comm.Recv(ANS, source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG, status=status)
        sender = status.source
        width = status.tag

        # Record the results if solution is closer to zero
        print "ANS: %g, SOL: %g" % (ANS[0], SOL[0])
        if np.absolute(ANS[0]) <= np.absolute(SOL[0]):
            SOL = np.copy(ANS)

        # Terminate when the subinterval width is reduced below 10E-11
        if ((1.0 / n_proc) / width <= INTERVAL):
            comm.Send(intervals[sender-1:sender+1], sender, tag=0)
            n_sent -= 1
        else:
            # Divide subinterval into smaller pieces for execution again
            comm.Send(intervals[sender-1:sender+1], sender, tag=width*2.0)

    # Master process prints the approximate zero of the function f to standard output
    print "The approximate solution: %1.15g, for r = %1.15g" % (SOL[0], SOL[1])


def slave(proc_id, comm):
    """The slave process receives the intervals to calculate and the width."""
    bounds = np.zeros(2, dtype=np.float64)
    ans = np.zeros(2, dtype=np.float)
    status = MPI.Status()
    comm.Recv(bounds, source=MASTER, tag=MPI.ANY_TAG, status=status)
    width = status.tag

    while width > 0:
        # Analyse the function values determined by each process to find a new
        # subinterval [a,b]c[0,1] within which the solution r is guaranteed to lie.
        r = np.linspace(bounds[0], bounds[1], width)
        results = f(r)  # Calculate the result of the function for each value of r

        # Find the result that is closest to 0 and return the r value
        min_idx = np.argmin(np.absolute(results))
        ans[0], ans[1] = results[min_idx], r[min_idx]

        # Return the results to master process and wait for more work
        comm.Send(ans, MASTER, tag=width)

        # Clean up memory used
        del(r)
        del(results)

        status = MPI.Status()
        comm.Recv(bounds, source=MASTER, tag=MPI.ANY_TAG, status=status)
        width = status.tag


if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    proc_id = comm.Get_rank()
    n_proc = comm.Get_size()

    if proc_id == MASTER:
        master(n_proc, comm)
    else:
        slave(proc_id, comm)
