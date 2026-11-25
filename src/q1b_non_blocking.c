#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
    int rank, size, next, prev,sent, received,error;
    MPI_Request request[2];
    // Initialize MPI
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        return 1;
    // Get process rank
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Get total number of processes
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Calculate next and previous process
    next = (rank + 1) % size;
    prev = (rank - 1 + size) % size;
    sent = rank;
    // Start receiving from previous process
    error = MPI_Irecv(&received, 1, MPI_INT, prev, 10, MPI_COMM_WORLD, &request[0]);
    if (error != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Start sending rank to next process
    error = MPI_Isend(&sent, 1, MPI_INT, next, 10, MPI_COMM_WORLD, &request[1]);
    if (error != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Wait for both communication operations to complete
    error = MPI_Waitall(2, request, MPI_STATUSES_IGNORE);
    if (error != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, 1);
    // Print communication result
    printf("Process %d: sent %d to P%d, received %d from P%d\n", rank, sent, next, received, prev);
    // Finalize MPI
    if (MPI_Finalize() != MPI_SUCCESS)
        return 1;
    return 0;
}
