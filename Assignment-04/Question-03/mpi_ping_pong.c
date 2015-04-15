/*
 * Assignment 4, Question 3 solution for MPI ping-pong timings to calcualate
 * alpha and beta, implemented in C using MPI.
 *
 * Copyright (C) 2015, Jonathan Gillett (100437638)
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

// Define process 0 as PING, process 1 as PONG
#define PING 0
#define PONG 1
// Number of trials for getting the average time
#define TRIALS 100


int main(int argc, char *argv[]) {
    int    proc_id;         // Process ID number
    int    n_proc;          // Number of processes
    int    N;               // The number of bytes to generate
    double timings[TRIALS]; // The timings for each trial
    MPI_Status status;

    if (argc < 2) {
        puts("ERROR: You must provide the number of bytes to send!");
        exit(1);
    }
    N = atoi(argv[1]);
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    if (n_proc > 2) {
        if (proc_id == PING) {
            puts("ERROR: Only two proceses (ping and pong).");
        }
        MPI_Finalize();
        exit(1);
    }  
    if (N < 1) {
        if (proc_id == PING) {
            puts("ERROR: You must specify the data size in bytes.");
        }
        MPI_Finalize();
        exit(1);
    }

    // The data to send back and forth, in bytes
    char *A = (char *) malloc(N*sizeof(char));
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < TRIALS; ++i) {
        if (proc_id == PING) {
            timings[i] = -MPI_Wtime();
            MPI_Send(A, N, MPI_CHAR, PONG, PING, MPI_COMM_WORLD);
            MPI_Recv(A, N, MPI_CHAR, MPI_ANY_SOURCE, PONG,
                     MPI_COMM_WORLD, &status);
            timings[i] += MPI_Wtime();
            // Simulate random sleeps to account for different scheduling
            usleep((rand() % 10000)+1000);
        }
        else {
            MPI_Recv(A, N, MPI_CHAR, MPI_ANY_SOURCE, PING,
                     MPI_COMM_WORLD, &status);
            MPI_Send(A, N, MPI_CHAR, PING, PONG, MPI_COMM_WORLD);
        }
    }

    if (proc_id == PING) {
        double sum = 0.0;
        for (int i = 0; i < TRIALS; ++i) {
            sum += timings[i];
        }
        printf("N bytes sent: %d, trials: %d, average time: %0.8f seconds\n",
               N, TRIALS, sum / TRIALS / 2.0);
    }
    MPI_Finalize();
    return 0;
}
