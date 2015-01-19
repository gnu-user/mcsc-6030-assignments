/******************************************************************************
 *
 * Sieve of Eratosthenes Implementation.
 *
 * Implementation based on the optimized implementation pseudocode provided on
 * Wikipedia: http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 * 
 * Copyright (C) 2014, Jonathan Gillett (100437638)
 * All rights reserved.
 *
 *****************************************************************************/
 #include <stdlib.h>
 #include <stdint.h>
 #include <stdbool.h>
 #include <stdio.h>
 #include <ctype.h>
 #include <string.h>
 #include <math.h>
 #include <getopt.h>


/*
 * Allocates the memory for the list of primes are returns
 * an array containing the list of prime numbers found
 * using the Sieve of Eratosthenes.
 */
void sieve(uint32_t n, uint32_t *primes)
{
    // Memory initialized to false with calloc
    bool *A = calloc(n-1, sizeof(bool));

    for (uint32_t i = 2; i <= (uint32_t) ceil(sqrt((double) n)); ++i)
    {
        if (! A[i-2])
        {
            uint32_t j = 0;
            for (uint32_t k = 0; k <= n; ++k)
            {
                j = (uint32_t) (pow((double) i, 2.0) + k*i);
                if (j > n)
                {
                    break;
                }
                A[j-2] = true;
            }
        }
    }

    for (uint32_t i = 0, j = 0; i < n-1; ++i)
    {
        if (! A[i])
        {
            primes[j] = i+2;
            ++j;
        }
    }
    free(A);
}


/* Prints the primes out comma separated */
void print_primes(uint32_t *primes, uint32_t num_primes)
{
    for (uint32_t i = 0; i < num_primes; ++i)
    {
        if (primes[i+1])
        {
            printf("%d, ", primes[i]);
        }
        else
        {
            printf("%d\n", primes[i]);
            break;
        }
    }
}

int main(int argc, char **argv)
{
    int c = 0;
    bool show_primes = true;
    uint32_t n = 0, num_primes = 0;
    uint32_t *primes = NULL;

    // Hack to get the first non-option argument N
    if (argc > 1)
    {
        if (isdigit(*argv[1]))
        {
            n = (uint32_t) atoi(argv[1]);
        }
    }

    while (1)
    {
        static struct option long_options[] =
        {
            {"print=", required_argument, 0, 0},
            {0,0,0,0}
        };
        int option_index = 0;
        c = getopt_long (argc, argv, "", long_options, &option_index);
        
        if (c == -1)
          break;

        switch (c)
        {
        case 0:
            if (long_options[option_index].flag != 0)
            {
                puts("breaking");
                break;
            }
            if (strcmp(long_options[option_index].name,"print=") == 0)
            {
                if (strcmp(optarg, "false") == 0 || strcmp(optarg, "False") == 0)
                {
                    show_primes = false;
                }
            }
            break;
        default:
            abort ();
        }
    }

    // Allocate the memory for the primes found using the prime-counting 
    // function, pi(x): http://en.wikipedia.org/wiki/Prime-counting_function
    num_primes = ((uint32_t) ceil(n / log((double) n)) + 2);
    primes = (uint32_t *) calloc(num_primes, sizeof(uint32_t));

    // If N provided calc primes, otherwise try highest N within time limit
    if (n >= 2)
    {
        sieve(n, primes);
        if (show_primes)
        {
            print_primes(primes, num_primes);
        }
    }
    
    /*
    for (int i = 0; i < num_primes; ++i)
    {
        printf("%d, ", primes[i]);
    }

    printf("\n");*/
    free(primes);

    return 0;
}