#include <stdio.h>
#include <omp.h>

#define N 1000
#define M 1000

int main() {
    double A[N][M], B[N][M];
    double start, end;

    // Initialize matrices
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            A[i][j] = i + j;

    // ---------------- Without collapse (outer loop only) ----------------
    start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            B[i][j] = (A[i][j] + A[i][j]) / 2.0;
        }
    }

    end = omp_get_wtime();
    printf("Without collapse: %f seconds\n", end - start);

    // ---------------- With collapse(2) ----------------
    start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            B[i][j] = (A[i][j] + A[i][j]) / 2.0;
        }
    }

    end = omp_get_wtime();
    printf("With collapse(2): %f seconds\n", end - start);

    return 0;
}
