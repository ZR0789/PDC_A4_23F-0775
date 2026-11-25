#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
    int rank, size, next, prev,sent, received,error;
    // Initialize MPI
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        return 1;
    // Get process rank
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Get total number of processes
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Calculate next and previous
    next = (rank + 1) % size;
    prev = (rank - 1 + size) % size;
    sent = rank;
    // Even processes send first
    if (rank % 2 == 0)
    {
        // Send rank to next process
        error = MPI_Send(&sent, 1, MPI_INT, next, 10, MPI_COMM_WORLD);
        if (error != MPI_SUCCESS)
            MPI_Abort(MPI_COMM_WORLD, 1);
        // Receive value from previous process
        error = MPI_Recv(&received, 1, MPI_INT, prev, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (error != MPI_SUCCESS)
            MPI_Abort(MPI_COMM_WORLD, 1);
    }
    // Odd processes receive first
    else
    {
        // Receive value from previous process
        error = MPI_Recv(&received, 1, MPI_INT, prev, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (error != MPI_SUCCESS)
            MPI_Abort(MPI_COMM_WORLD, 1);
        // Send rank to next process
        error = MPI_Send(&sent, 1, MPI_INT, next, 10, MPI_COMM_WORLD);
        if (error != MPI_SUCCESS)
            MPI_Abort(MPI_COMM_WORLD, 1);
    }
    // Print communication result
    printf("Process %d: sent %d to P%d, received %d from P%d\n", rank, sent, next, received, prev);
    // Finalize MPI
    if (MPI_Finalize() != MPI_SUCCESS)
        return 1;
    return 0;
}
