// Sequential version
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500   // you can change the matrix size

int main() {
    int i, j, k;
    double A[N][N], B[N][N], C[N][N];

    // Initialize matrices
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }

    double start = omp_get_wtime();

    // Matrix Multiplication (sequential)
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];

    double end = omp_get_wtime();

    printf("Time (Sequential): %f seconds\n", end - start);
    return 0;
}

// Parallel version


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500

int main() {
    int i, j, k;
    double A[N][N], B[N][N], C[N][N];

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }

    double start = omp_get_wtime();

    #pragma omp parallel for private(i, j, k) shared(A, B, C) collapse(2)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            double sum = 0;
            for (k = 0; k < N; k++)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }

    double end = omp_get_wtime();
    printf("Time (Parallel): %f seconds\n", end - start);
    return 0;
}
