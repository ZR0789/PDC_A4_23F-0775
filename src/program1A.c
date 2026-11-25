#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
    int rank, size, x;
    int error;
    // Initialize MPI
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    {
        printf("Error initializing MPI.\n");
        return 1;
    }
    // Get the rank of the current process
    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }
    // Get the total number of processes
    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }
    // Check that exactly 4 processes are running
    if (size != 4)
    {
        if (rank == 0)
            printf("Run with exactly 4 processes.\n");
        MPI_Finalize();
        return 0;
    }
    if (rank == 0)
    {
        x = 42;
        // Send 42 to Process 1 with tag 1
        error = MPI_Send(&x, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        if (error != MPI_SUCCESS)
        {
            printf("P0: Error sending message to P1.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("P0: Sent %d to P1\n", x);
    }
    else if (rank == 1)
    {
        // Receive value from Process 0 with tag 1
        error = MPI_Recv(&x, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (error != MPI_SUCCESS)
        {
            printf("P1: Error receiving message from P0.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("P1: Received %d from P0\n", x);
        // Multiply received value by 2
        x = x * 2;
        // Send result to Process 2 with tag 2
        error = MPI_Send(&x, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);
        if (error != MPI_SUCCESS)
        {
            printf("P1: Error sending message to P2.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("P1: Sent %d to P2\n", x);
    }
    else if (rank == 2)
    {
        // Receive value from Process 1 with tag 2
        error = MPI_Recv(&x, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (error != MPI_SUCCESS)
        {
            printf("P2: Error receiving message from P1.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("P2: Received %d from P1\n", x);
        // Add 10 to received value
        x = x + 10;
        // Send result to Process 3 with tag 3
        error = MPI_Send(&x, 1, MPI_INT, 3, 3, MPI_COMM_WORLD);
        if (error != MPI_SUCCESS)
        {
            printf("P2: Error sending message to P3.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("P2: Sent %d to P3\n", x);
    }
    else if (rank == 3)
    {
        // Receive final value from Process 2 with tag 3
        error = MPI_Recv(&x, 1, MPI_INT, 2, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (error != MPI_SUCCESS)
        {
            printf("P3: Error receiving message from P2.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        // Print final result
        printf("P3: Received final value = %d\n", x);
    }
    // Terminate MPI execution
    if (MPI_Finalize() != MPI_SUCCESS)
    {
        return 1;
    }
    return 0;
}

