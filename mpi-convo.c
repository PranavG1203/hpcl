#include <mpi.h>
#include <stdio.h>

#define N 5 // image size
#define K 3 // kernel size

int main(int argc, char *argv[])
{
    int rank, size;
    int image[N][N] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}};

    int kernel[K][K] = {
        {1, 0, -1},
        {1, 0, -1},
        {1, 0, -1}};

    int output[N][N] = {0};
    int local_image[N][N];
    int local_output[N][N] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = N / size;

    // Scatter image rows to all processes
    MPI_Scatter(image, rows_per_process * N, MPI_INT,
                local_image, rows_per_process * N, MPI_INT,
                0, MPI_COMM_WORLD);

    // Broadcast kernel to all
    MPI_Bcast(kernel, K * K, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform convolution on local rows
    for (int i = 1; i < rows_per_process - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++)
            {
                for (int kj = -1; kj <= 1; kj++)
                {
                    sum += local_image[i + ki][j + kj] * kernel[ki + 1][kj + 1];
                }
            }
            local_output[i][j] = sum;
        }
    }

    // Gather results
    MPI_Gather(local_output, rows_per_process * N, MPI_INT,
               output, rows_per_process * N, MPI_INT,
               0, MPI_COMM_WORLD);

    // Print final output (only process 0)
    if (rank == 0)
    {
        printf("Convolved Output Matrix:\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%4d", output[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
