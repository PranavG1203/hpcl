#include <stdio.h>
#include <omp.h>

int main() {
    long long num_steps = 100000000;
    double step, x, pi, sum = 0.0;
    step = 1.0 / (double) num_steps;

    double start = omp_get_wtime();

    #pragma omp parallel for private(x) reduction(+:sum)
    for (int i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    pi = step * sum;
    double end = omp_get_wtime();

    printf("Calculated Pi = %f\n", pi);
    printf("Time = %f seconds using %d threads\n", end - start, omp_get_max_threads());
    return 0;
}
