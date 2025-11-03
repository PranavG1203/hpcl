// Sequential version
#include <stdio.h>
#include <omp.h>

#define N 1000

int main() {
    int i, j;
    double scalar = 3.5;
    double A[N][N], C[N][N];

    // Initialize matrix
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            A[i][j] = i + j;

    double start = omp_get_wtime();

    // Sequential multiplication
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            C[i][j] = scalar * A[i][j];

    double end = omp_get_wtime();
    printf("Time (Sequential): %f seconds\n", end - start);
    return 0;
}

// Parallel version

#include <stdio.h>
#include <omp.h>

#define N 1000

int main() {
    int i, j;
    double scalar = 3.5;
    double A[N][N], C[N][N];

    // Initialize matrix
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            A[i][j] = i + j;

    double start = omp_get_wtime();

    // Parallelized matrix-scalar multiplication
    #pragma omp parallel for private(i, j) shared(A, C, scalar) schedule(static, 10)
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            C[i][j] = scalar * A[i][j];

    double end = omp_get_wtime();
    printf("Time (Parallel): %f seconds\n", end - start);
    return 0;
}
