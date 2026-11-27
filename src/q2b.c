#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int is_prime(int n)
{
    if (n < 2)
        return 0;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int rank, p,range[2], a, b, total_numbers, chunk, remainder,start, end;
    int local_count = 0;
    int total_count = 0;
    int *counts = NULL;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    // Root reads the range
    if (rank == 0)
    {
        if (argc != 3)
        {
            printf("Usage: mpirun -np 4 ./q2b 2 100000\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        range[0] = atoi(argv[1]);
        range[1] = atoi(argv[2]);
        if (range[0] > range[1])
        {
            printf("Invalid range.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    // Broadcast both values in one call
    MPI_Bcast(range, 2, MPI_INT, 0, MPI_COMM_WORLD);
    a = range[0];
    b = range[1];
    // Calculate the number of integers
    total_numbers = b - a + 1;
    chunk = total_numbers / p;
    remainder = total_numbers % p;
    // Calculate this process's sub-range
    start = a + rank * chunk;
    if (rank < remainder)
        start += rank;
    else
        start += remainder;
    end = start + chunk - 1;
    if (rank < remainder)
        end++;
    // Count prime numbers in the assigned range
    for (int i = start; i <= end; i++)
    {
        if (is_prime(i))
        {
            local_count++;
        }
    }
    // Root allocates space for all process counts
    if (rank == 0)
    {
        counts = (int *)malloc(p * sizeof(int));
        if (counts == NULL)
        {
            printf("Memory allocation failed.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    // Collect local counts at root
    MPI_Gather(&local_count, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // Root displays the results
    if (rank == 0)
    {
        printf("\nPrime counts by process:\n");
        for (int i = 0; i < p; i++)
        {
            printf("Process %d: %d primes\n", i, counts[i]);
            total_count += counts[i];
        }
        printf("\nTotal primes between %d and %d: %d\n", a, b, total_count);
        free(counts);
    }
    MPI_Finalize();
    return 0;
}
