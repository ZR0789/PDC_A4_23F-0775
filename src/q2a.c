
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 1000000
int main(int argc, char *argv[])
{
    int rank, p;
    int *arr = NULL;
    int *local_arr, chunk;
    long long local_sum = 0;
    long long global_sum = 0;
    long long expected_sum;
    double start_time, end_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    // Divide array equally among processes
    chunk = N / p;
    // Every process allocates space for its chunk
    local_arr = (int *)malloc(chunk * sizeof(int));

    if (local_arr == NULL)
    {
        printf("Memory allocation failed.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    // Root initializes the complete array
    if (rank == 0)
    {
        arr = (int *)malloc(N * sizeof(int));
        if (arr == NULL)
        {
            printf("Memory allocation failed.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        for (int i = 0; i < N; i++)
        {
            arr[i] = i + 1;
        }
    }
    // Start timing after all processes are ready
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    // Distribute equal chunks among processes
    MPI_Scatter(arr, chunk, MPI_INT, local_arr, chunk, MPI_INT, 0, MPI_COMM_WORLD);
    // Each process calculates its local sum
    for (int i = 0; i < chunk; i++)
    {
        local_sum += local_arr[i];
    }
    // Collect all local sums at root
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    // End timing after reduction completes
    end_time = MPI_Wtime();
    // Root verifies and displays the result
    if (rank == 0)
    {
        expected_sum = (long long)N * (N + 1) / 2;
        printf("\nNumber of processes: %d\n", p);
        printf("Calculated Sum: %lld\n", global_sum);
        printf("Expected Sum:   %lld\n", expected_sum);
        if (global_sum == expected_sum)
        {
            printf("Result: PASS\n");
        }
        else
        {
            printf("Result: FAIL\n");
        }
        printf("Elapsed Time: %f seconds\n",end_time - start_time);
        free(arr);
    }

    free(local_arr);
    MPI_Finalize();
    return 0;
}
