#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Function to check if a number is prime
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main() {
    int start_range = 1, end_range = 1000000; // Range to test
    int count_static = 0, count_dynamic = 0;
    double start_time, end_time;

    printf("Counting prime numbers between %d and %d\n\n", start_range, end_range);

    // ------------------- Static Scheduling -------------------
    start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:count_static) schedule(static)
    for (int i = start_range; i <= end_range; i++) {
        if (is_prime(i))
            count_static++;
    }

    end_time = omp_get_wtime();
    printf("[STATIC SCHEDULING]\n");
    printf("Total primes = %d\n", count_static);
    printf("Execution time = %f seconds\n\n", end_time - start_time);

    // ------------------- Dynamic Scheduling -------------------
    start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:count_dynamic) schedule(dynamic, 100)
    for (int i = start_range; i <= end_range; i++) {
        if (is_prime(i))
            count_dynamic++;
    }

    end_time = omp_get_wtime();
    printf("[DYNAMIC SCHEDULING]\n");
    printf("Total primes = %d\n", count_dynamic);
    printf("Execution time = %f seconds\n\n", end_time - start_time);

    // ------------------- Verification -------------------
    if (count_static == count_dynamic)
        printf("Verification: PASSED ✅\n");
    else
        printf("Verification: FAILED ❌\n");

    return 0;
}
