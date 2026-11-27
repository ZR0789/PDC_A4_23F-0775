#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define M 8
int main(int argc, char *argv[])
{
    int rank, p;
    int A[M][M], x[M], y[M];
    int rows, *local_A, *local_y;
    int serial_y[M], correct = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (M % p != 0)
    {
        if (rank == 0)
            printf("Number of processes must divide 8 evenly.\n");
        MPI_Finalize();
        return 1;
    }
    rows = M / p;
    local_A = (int *)malloc(rows * M * sizeof(int));
    local_y = (int *)malloc(rows * sizeof(int));
    if (local_A == NULL || local_y == NULL)
    {
        printf("Memory allocation failed.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (rank == 0)
    {
        for (int i = 0; i < M; i++)
        {
            x[i] = i + 1;
            for (int j = 0; j < M; j++)
                A[i][j] = i + j;
        }
    }
    MPI_Bcast(x, M, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, rows * M, MPI_INT, local_A, rows * M, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < rows; i++)
    {
        local_y[i] = 0;
        for (int j = 0; j < M; j++)
            local_y[i] += local_A[i * M + j] * x[j];
    }
    MPI_Gather(local_y, rows, MPI_INT, y, rows, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("\nParallel Result:\n");
        for (int i = 0; i < M; i++)
            printf("%d ", y[i]);
        printf("\n");
        for (int i = 0; i < M; i++)
        {
            serial_y[i] = 0;
            for (int j = 0; j < M; j++)
                serial_y[i] += (i + j) * x[j];
        }
        printf("\nSerial Result:\n");
        for (int i = 0; i < M; i++)
            printf("%d ", serial_y[i]);
        printf("\n");
        for (int i = 0; i < M; i++)
        {
            if (y[i] != serial_y[i])
            {
                correct = 0;
                break;
            }
        }
        if (correct)
            printf("\nVerification: PASS\n");
        else
            printf("\nVerification: FAIL\n");
    }
    free(local_A);
    free(local_y);
    MPI_Finalize();
    return 0;
}
