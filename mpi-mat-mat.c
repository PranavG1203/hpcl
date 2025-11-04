#include <mpi.h>
#include <stdio.h>

#define N 4  // assuming 4x4 matrices

int main(int argc, char* argv[]) {
    int rank, size;
    int A[N][N] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    int B[N][N] = {
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1}
    };
    int C[N][N];        // final result
    int local_A[N][N];  // partial A rows
    int local_C[N][N];  // partial result

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = N / size;

    // Scatter matrix A rows
    MPI_Scatter(A, rows_per_process * N, MPI_INT,
                local_A, rows_per_process * N, MPI_INT,
                0, MPI_COMM_WORLD);

    // Broadcast entire matrix B to all
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute partial matrix multiplication
    for (int i = 0; i < rows_per_process; i++) {
        for (int j = 0; j < N; j++) {
            local_C[i][j] = 0;
            for (int k = 0; k < N; k++)
                local_C[i][j] += local_A[i][k] * B[k][j];
        }
    }

    // Gather all parts of result matrix
    MPI_Gather(local_C, rows_per_process * N, MPI_INT,
               C, rows_per_process * N, MPI_INT,
               0, MPI_COMM_WORLD);

    // Print final result (only rank 0)
    if (rank == 0) {
        printf("Resultant Matrix C:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                printf("%4d", C[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
