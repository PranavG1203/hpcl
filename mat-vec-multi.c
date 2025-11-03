// Sequential version

#include <stdio.h>
#include <omp.h>

#define N 1000

int main() {
    double A[N][N], x[N], y[N];
    int i, j;

    // Initialize A and x
    for (i = 0; i < N; i++) {
        x[i] = i * 0.5;
        for (j = 0; j < N; j++)
            A[i][j] = i + j;
    }

    double start = omp_get_wtime();

    // Sequential Matrix-Vector multiplication
    for (i = 0; i < N; i++) {
        y[i] = 0.0;
        for (j = 0; j < N; j++)
            y[i] += A[i][j] * x[j];
    }

    double end = omp_get_wtime();
    printf("Sequential Time: %f sec\n", end - start);
    return 0;
}


// Parallel version

#include <stdio.h>
#include <omp.h>

#define N 1000

int main() {
    double A[N][N], x[N], y[N];
    int i, j;

    // Initialize A and x
    for (i = 0; i < N; i++) {
        x[i] = i * 0.5;
        for (j = 0; j < N; j++)
            A[i][j] = i + j;
    }

    double start = omp_get_wtime();

    #pragma omp parallel for private(i, j) shared(A, x, y)
    for (i = 0; i < N; i++) {
        double sum = 0.0;
        for (j = 0; j < N; j++)
            sum += A[i][j] * x[j];
        y[i] = sum;
    }

    double end = omp_get_wtime();
    printf("Parallel Time: %f sec\n", end - start);
    return 0;
}
