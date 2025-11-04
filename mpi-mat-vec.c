#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int m = 4, n = 4; // matrix dimensions
    int A[4][4] = {{1, 2, 3, 4},
                   {5, 6, 7, 8},
                   {9, 10, 11, 12},
                   {13, 14, 15, 16}};
    int B[4] = {1, 1, 1, 1};       // vector
    int local_A[2][4], local_C[2]; // assuming 2 rows per process for 2 processes
    int C[4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = m / size;

    // Scatter rows of matrix A to all processes
    MPI_Scatter(A, rows_per_process * n, MPI_INT,
                local_A, rows_per_process * n, MPI_INT,
                0, MPI_COMM_WORLD);

    // Broadcast vector B to all processes
    MPI_Bcast(B, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes its part of the result
    for (int i = 0; i < rows_per_process; i++)
    {
        local_C[i] = 0;
        for (int j = 0; j < n; j++)
            local_C[i] += local_A[i][j] * B[j];
    }

    // Gather results from all processes to form final C
    MPI_Gather(local_C, rows_per_process, MPI_INT,
               C, rows_per_process, MPI_INT,
               0, MPI_COMM_WORLD);

    // Display final result vector
    if (rank == 0)
    {
        printf("Result vector C = [ ");
        for (int i = 0; i < m; i++)
            printf("%d ", C[i]);
        printf("]\n");
    }

    MPI_Finalize();
    return 0;
}
