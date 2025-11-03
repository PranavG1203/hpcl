#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000         // Size of the square matrix
#define BLOCK_SIZE 32  // Block size for cache-friendly version

int main() {
    static double A[N][N], B[N][N];
    double start, end;

    // ---------------- Initialize Matrix ----------------
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = i * N + j; // simple unique values

    // ---------------- Normal Parallel Transpose ----------------
    start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[j][i] = A[i][j];
        }
    }

    end = omp_get_wtime();
    printf("Normal Transpose Time: %f seconds\n", end - start);

    // ---------------- Cache-Friendly Blocked Transpose ----------------
    start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int ii = 0; ii < N; ii += BLOCK_SIZE) {
        for (int jj = 0; jj < N; jj += BLOCK_SIZE) {
            for (int i = ii; i < ii + BLOCK_SIZE && i < N; i++) {
                for (int j = jj; j < jj + BLOCK_SIZE && j < N; j++) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }

    end = omp_get_wtime();
    printf("Blocked Transpose Time: %f seconds\n", end - start);

    // ---------------- Verify correctness for small matrix ----------------
    int correct = 1;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (B[j][i] != A[i][j]) {
                correct = 0;
                break;
            }
        }
    }

    if (correct)
        printf("Verification: PASSED ✅\n");
    else
        printf("Verification: FAILED ❌\n");

    return 0;
}
