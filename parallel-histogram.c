#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000   // Size of array
#define BINS 10      // Number of histogram bins (0–9)

int main() {
    int *data = (int*)malloc(N * sizeof(int));
    int hist_serial[BINS] = {0};
    int hist_critical[BINS] = {0};
    int hist_atomic[BINS] = {0};
    int hist_reduction[BINS] = {0};
    double start, end;

    // ---------------- Initialize Random Data ----------------
    for (int i = 0; i < N; i++)
        data[i] = rand() % BINS;  // Values between 0 and BINS-1

    printf("Parallel Histogram of %d elements with %d bins\n\n", N, BINS);

    // ---------------- Serial Version (for comparison) ----------------
    start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        hist_serial[data[i]]++;
    }
    end = omp_get_wtime();
    printf("[Serial] Time: %f seconds\n", end - start);

    // ---------------- Parallel Version (using critical) ----------------
    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int val = data[i];
        #pragma omp critical
        {
            hist_critical[val]++;
        }
    }
    end = omp_get_wtime();
    printf("[Critical] Time: %f seconds\n", end - start);

    // ---------------- Parallel Version (using atomic) ----------------
    start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int val = data[i];
        #pragma omp atomic
        hist_atomic[val]++;
    }
    end = omp_get_wtime();
    printf("[Atomic] Time: %f seconds\n", end - start);

    // ---------------- Parallel Version (using reduction) ----------------
    // OpenMP doesn't directly support reduction on arrays,
    // so we use a manual per-thread private array and then combine them.
    start = omp_get_wtime();
    #pragma omp parallel
    {
        int local_hist[BINS] = {0};
        #pragma omp for nowait
        for (int i = 0; i < N; i++) {
            local_hist[data[i]]++;
        }

        #pragma omp critical
        {
            for (int j = 0; j < BINS; j++) {
                hist_reduction[j] += local_hist[j];
            }
        }
    }
    end = omp_get_wtime();
    printf("[Manual Reduction] Time: %f seconds\n", end - start);

    // ---------------- Verification ----------------
    int correct = 1;
    for (int i = 0; i < BINS; i++) {
        if (hist_serial[i] != hist_critical[i] ||
            hist_serial[i] != hist_atomic[i] ||
            hist_serial[i] != hist_reduction[i]) {
            correct = 0;
            break;
        }
    }

    if (correct)
        printf("\nVerification: PASSED ✅\n");
    else
        printf("\nVerification: FAILED ❌\n");

    free(data);
    return 0;
}
