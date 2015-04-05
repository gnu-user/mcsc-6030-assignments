/*
 * Assignment 4, Question 4 solution using MPI in C.
 *
 * Copyright (C) 2014, Jonathan Gillett (100437638)
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Define process 0 as MASTER
#define MASTER 0
// Define the subinterval limit
#define INTERVAL 10E-11
// Set the width for the search space of each subinterval
#define WIDTH 1E6
// message tags
#define REQUEST  1
#define REPLY    2

double f(double x) {
    return -2.0 + sin(x) + sin(pow(x, 2)) + sin(pow(x, 3))
           + sin(pow(x, 999)) + sin(pow(x, 1000)); 
}


// Sets the intervals given the bounds and number of subintervals
void set_intervals(double intervals[], double left, double right, int n) {
    double h = ((right - left ) / n);
    for (int i = 0; i < n; ++i) {
        intervals[i] = left + h*i;
    }
}


// The master process divides the interval [0, 1] into subintervals
void master(int n_proc) {
    double *intervals = (double *) malloc(n_proc*sizeof(double));
    // Divide the interval [0,1] into several subintervals for each process
    set_intervals(intervals, 0.0, 1.0, n_proc);
    double ANS[4];                 // The result, r value, bounds from processes
    double SOL[4] = { HUGE_VAL };  // The final smallest solution, r value, bounds
    int n_sent = 0;
    MPI_Status status;

    // Assign to each task an endpoint of one of the subintervals at which
    // the function f should be evaluated
    for (int i = 1; i <= n_proc; ++i) {
        double bounds[2] = { intervals[i-1], intervals[i] };
        MPI_Send(bounds, 2, MPI_DOUBLE, i, REQUEST, MPI_COMM_WORLD);
        n_sent++;
    }

    while (n_sent > 0) {
        // Receive the r value and result that closest approximates function f(x)
        MPI_Recv(ANS, 4, MPI_DOUBLE, MPI_ANY_SOURCE, REQUEST,
                 MPI_COMM_WORLD, &status);

        // Analyse the function values determined by each process to find a new
        // subinterval [a,b]c[0,1] within the solution r is guaranteed to lie
        if (fabs(ANS[0]) <= fabs(SOL[0])) {
            SOL[0] = ANS[0];
            SOL[1] = ANS[1];
            SOL[2] = ANS[2];
            SOL[3] = ANS[3];
            // Update the subintervals for the search space
            set_intervals(intervals, ANS[2], ANS[3], n_proc);
        }

        // Terminate when the subinterval width is reduced below 10E-11
        if ((ANS[3] - ANS[2]) / WIDTH <= INTERVAL) {
            double bounds[2] = { -1.0, -1.0 };
            MPI_Send(bounds, 2, MPI_DOUBLE, status.MPI_SOURCE, WIDTH, MPI_COMM_WORLD);
            n_sent--;
        }
        // Divide subinterval into smaller pieces for execution again
        else {
            double bounds[2] = { intervals[status.MPI_SOURCE-1],
                                 intervals[status.MPI_SOURCE] };
            MPI_Send(bounds, 2, MPI_DOUBLE, status.MPI_SOURCE, WIDTH, MPI_COMM_WORLD);
        }
    }
    free(intervals);
    printf("Approx zero of function f(x): %1.15g\nr value: %1.15g", SOL[0], SOL[1]);
}


// The slave process receives the intervals to calculate and the width
void slave(int proc_id) {
    double bounds[2];
    double ans[4] = { HUGE_VAL };
    double h, r, result;
    MPI_Status status;
    MPI_Recv(bounds, 2, MPI_DOUBLE, MPI_ANY_SOURCE, REQUEST,
             MPI_COMM_WORLD, &status);

    while (bounds[0] >= 0) {
        h = (bounds[1] - bounds[0]) / WIDTH;
        // Given the width iterate through each value of r
        // to find the result that is closest to 0
        for (int i = 0; i < WIDTH; ++i) {
            r = bounds[0] + i*h;
            result = f(r);
            if (ans[0] >= fabs(result)) {
                ans[0] = result;
                ans[1] = r;
                ans[2] = bounds[0];
                ans[3] = bounds[1];
            }
        }
        // Return the results to master process and wait for more work
        MPI_Send(ans, 4, MPI_DOUBLE, status.MPI_SOURCE, REPLY, MPI_COMM_WORLD);
        MPI_Recv(bounds, 2, MPI_DOUBLE, MPI_ANY_SOURCE, REQUEST, 
                 MPI_COMM_WORLD, &status);
    }
}


int main(int argc, char *argv[]) {
    int    proc_id;      // Process ID number
    int    n_proc;       // Number of processes

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    MPI_Barrier(MPI_COMM_WORLD);

    if (proc_id == MASTER) {
        puts("Master!");
        master(n_proc);
    }
    else {
        puts("Slave!");
        slave(proc_id);
    }

    MPI_Finalize();
    return 0;
}
