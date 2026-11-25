#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
    int rank, size, local_data[10],recv_buf[10],j, error;
    // Initialize MPI
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        return 1;
    // Get process rank
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Get total number of processes
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Check array capacity
    if (size > 10)
    {
        if (rank == 0)
            printf("Maximum 10 processes allowed.\n");
        MPI_Finalize();
        return 0;
    }
    // Initialize local data and receive buffer
    for (j = 0; j < size; j++)
    {
        local_data[j] = rank * 10 + j;
        recv_buf[j] = -1;
    }
    // Exchange data with every other process
    for (j = 0; j < size; j++)
    {
        if (rank != j)
        {
            if (rank < j)
            {
                // Lower-ranked process sends first
                error = MPI_Send(&local_data[j], 1, MPI_INT, j, 20, MPI_COMM_WORLD);
                if (error != MPI_SUCCESS)
                    MPI_Abort(MPI_COMM_WORLD, 1);
                // Then receives data from higher-ranked process
                error = MPI_Recv(&recv_buf[j], 1, MPI_INT, j, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (error != MPI_SUCCESS)
                    MPI_Abort(MPI_COMM_WORLD, 1);
            }
            else
            {
                // Higher-ranked process receives first
                error = MPI_Recv(&recv_buf[j], 1, MPI_INT, j, 20, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (error != MPI_SUCCESS)
                    MPI_Abort(MPI_COMM_WORLD, 1);
                // Then sends data to lower-ranked process
                error = MPI_Send(&local_data[j], 1, MPI_INT, j, 20, MPI_COMM_WORLD);
                if (error != MPI_SUCCESS)
                    MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }
    }
    // Print the full receive buffer
    printf("Process %d: recv_buf = ", rank);
    for (j = 0; j < size; j++)
    {
        printf("%d ", recv_buf[j]);
    }
    printf("\n");
    // Finalize MPI
    if (MPI_Finalize() != MPI_SUCCESS)
        return 1;
    return 0;
}
