#!/bin/bash
###############################################################################
#
# Assignment 4, Question 3 runner to execute each benchmark with increasing
# sizes in bytes and display the result
#
# Copyright (C) 2015, Jonathan Gillett (100437638)
# All rights reserved.
#
###############################################################################
SIZES=( 1000 10000 100000 1000000 10000000 100000000 )
PYBIN="python2 ./mpi_ping_pong.py"
CBIN="./mpi_ping_pong.exe"

echo -e "\nEXECUTING PYTHON BENCHMARKS"
echo "========================================="
for size in "${SIZES[@]}"
do
    mpiexec -np 2 ${PYBIN} ${size}
done

echo -e "\n\nEXECUTING C BENCHMARKS"
echo "========================================="
for size in "${SIZES[@]}"
do
    mpiexec -np 2 ${CBIN} ${size}
done