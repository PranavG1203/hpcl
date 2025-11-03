#include <stdio.h>
#include <omp.h>

int main() {
    int N = 100000000; // Number of terms (adjust for testing)
    long long sum_no_reduction = 0;
    long long sum_with_reduction = 0;
    double start, end;

    printf("Computing sum of squares of first %d natural numbers\n", N);

    // ---------------- Without reduction ----------------
    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        // Each thread updates shared variable -> data race
        sum_no_reduction += (long long)i * i;
    }
    end = omp_get_wtime();
    printf("\n[Without Reduction]");
    printf("\nSum (may be incorrect due to race conditions): %lld", sum_no_reduction);
    printf("\nTime taken: %f seconds\n", end - start);

    // ---------------- With reduction ----------------
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_with_reduction)
    for (int i = 1; i <= N; i++) {
        sum_with_reduction += (long long)i * i;
    }
    end = omp_get_wtime();
    printf("\n[With Reduction]");
    printf("\nSum (correct): %lld", sum_with_reduction);
    printf("\nTime taken: %f seconds\n", end - start);

    return 0;
}
