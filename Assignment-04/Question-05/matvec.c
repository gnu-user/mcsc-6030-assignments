/*
 * Assignment 4, Question 5 matvec implementation in C using MPI.
 *
 * Copyright (C) 2015, Jonathan Gillett (100437638)
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cblas.h>
#include <mpi.h>

// Minimum of two values
#define min(a, b) (((a) < (b)) ? (a) : (b))
// Define process 0 as MASTER
#define MASTER 0
// Message tags
#define REQUEST 1
#define REPLY   2


// Master process, divides up the matrix into rows for computation by slaves
void master(int n_proc, int n_rows, int n_cols) {
    // 0. Allocate space for arrays as needed to compute y = A*x
    double *x = (double *) malloc(n_cols*sizeof(double));
    double *y = (double *) malloc(n_rows*sizeof(double));
    double *z = (double *) malloc(n_rows*sizeof(double));
    double *A = (double *) malloc(n_rows*n_cols*sizeof(double));
    double *sub = (double *) malloc(n_cols*sizeof(double));
    double ans = 0.0;
    int n_sent = 0, row = 0, sender = 0;
    MPI_Status status;

    // 1. Define data A (n_rows x n_cols matrix) and x (n_cols x 1 vector)
    for (int i = 0; i < n_cols; ++i) {
        x[i] = i+1;
    } 
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            A[(i*n_cols)+j] = i+1;
        }
    }
    
    // 2. Broadcast vector x to all processes
    MPI_Bcast(x, n_cols, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    // 3. Send first rows to other processes
    for (int i = 1; i < min(n_rows+1, n_proc); ++i) {
    	memcpy((void *) sub, (void *) &A[n_sent*n_cols], n_cols * sizeof(double));
        MPI_Send(sub, n_cols, MPI_DOUBLE, i, n_sent, MPI_COMM_WORLD);
        n_sent++;
    }

    // Now loop over remaining rows, receiving dot products computed
    for (int i = 0; i < n_rows; ++i) {
        // Receive a computed inner product from one of the processes
        MPI_Recv(&ans, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        sender = status.MPI_SOURCE;
        row = status.MPI_TAG;
        y[row] = ans;

        // Either send another row back to sender or send final y=Ax
        // along with a tag to signal completion.
        if (n_sent < n_rows) {
        	memcpy((void *) sub, (void *) &A[n_sent*n_cols], n_cols * sizeof(double));
            MPI_Send(sub, n_cols, MPI_DOUBLE, sender, n_sent, MPI_COMM_WORLD);
            n_sent++;
        }
        // Inform sender that there is no more work
        else {
            MPI_Send(y, n_rows, MPI_DOUBLE, sender, n_rows+1, MPI_COMM_WORLD);
        }
    }

    printf("Finished!\n");
    // Calculate the norm to verify ||y-Ax|| is correct
    cblas_dgemv(CblasRowMajor, CblasNoTrans, n_rows, n_cols, 1.0, A, n_cols,
	      		x, 1, 0.0, z, 1);
    for (int i = 0; i < n_rows; ++i) {
    	y[i] -= z[i];
    }
    printf("Check: ||y-Ax|| = %f\n", cblas_dnrm2(n_rows, y, 1));

    free(x);
    free(y);
    free(z);
    free(A);
    free(sub);
}


// Slave process computes inner product and returns result to master process
void slave(int proc_id, int n_rows, int n_cols) {
    // Allocate arrays for receiving and sending rows & dot products
    double *x = (double *) malloc(n_cols*sizeof(double));
    double *my_row = (double *) malloc(n_cols*sizeof(double));
    double ans = 0.0;
    int row = 0;
    MPI_Status status;

    // 1. Receive vector x from master process
    MPI_Bcast(x, n_cols, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    // Receive row to calculate from master process
    MPI_Recv(my_row, n_cols, MPI_DOUBLE, MASTER, MPI_ANY_TAG, 
             MPI_COMM_WORLD, &status);
    row = status.MPI_TAG;

    while (row < n_rows) {
        // Compute inner product and return result to master process
        ans = cblas_ddot(n_cols, my_row, 1, x, 1);
        MPI_Send(&ans, 1, MPI_DOUBLE, MASTER, row, MPI_COMM_WORLD);
        // Receive new row from master
        MPI_Recv(my_row, n_cols, MPI_DOUBLE, MASTER, MPI_ANY_TAG, 
                 MPI_COMM_WORLD, &status);
        row = status.MPI_TAG;
    }
}  


int main(int argc, char *argv[]) {
    int    proc_id;       // Process ID number
    int    n_proc;        // Number of processes
    int    n_rows = 1000; // Number of rows
    int    n_cols = 1000; // Number of columns

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    if (n_proc == 1) {
        puts("Error: requires two or more processes (no work will be done).");
        MPI_Finalize();
        exit(1);
    }

    if (proc_id == MASTER) {
        master(n_proc, n_rows, n_cols);
    }
    else {
        slave(proc_id, n_rows, n_cols);
    }

    MPI_Finalize();
    return 0;
}
