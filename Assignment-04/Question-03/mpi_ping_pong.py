#!/usr/bin/env python2
###############################################################################
#
# Assignment 4, Question 3 solution for MPI ping-pong timings to calcualate
# alpha and beta, implemented in Python using MPI.
#
# Copyright (C) 2015, Jonathan Gillett (100437638)
# All rights reserved.
#
###############################################################################
import numpy as np
import sys
from mpi4py import MPI
from time import sleep
from random import random

# Define process 0 as PING, process 1 as PONG
PING = 0
PONG = 1
# Number of trials for getting the average time
TRIALS = 100

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "ERROR: You must provide the number of bytes to send!."
        sys.exit()
    N = int(sys.argv[1])  # The number of bytes to generate
    comm = MPI.COMM_WORLD
    proc_id = comm.Get_rank()
    n_proc = comm.Get_size()
    status = MPI.Status()

    # Error checking only 2 processes can be used
    if n_proc > 2:
        if proc_id == PING:
            print "ERROR: Only two proceses (ping and pong)."
        MPI.Finalize()
        sys.exit()

    if N < 1:
        if proc_id == PING:
            print "ERROR: You must specify the data size in bytes."
        MPI.Finalize()
        sys.exit()

    # The data to send back and forth, in bytes
    A = np.empty(N, dtype=np.int8)
    comm.Barrier()

    # Send the data back and forth 100 times to get the average time
    timings = []
    for i in range(0, 100):
        if proc_id == PING:
            local_time = -MPI.Wtime()
            comm.Send(A, PONG, tag=PING)
            comm.Recv(A, source=MPI.ANY_SOURCE, tag=PONG, status=status)
            timings.append(local_time + MPI.Wtime())
            # Simulate random sleeps to account for different scheduling
            sleep(random() / 100)
        else:
            comm.Recv(A, source=MPI.ANY_SOURCE, tag=PING, status=status)
            comm.Send(A, PING, tag=PONG)

    if proc_id == PING:
        print "N bytes sent: %d, trials: %d, average time: %0.8f seconds" \
              % (N, TRIALS, sum(timings) / float(len(timings)) / 2.0)
