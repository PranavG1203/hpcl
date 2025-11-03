#include <stdio.h>
#include <omp.h>

int main()
{
    long long int num_operations = 1000000000; // 1 billion ops
    double a = 1.0, b = 2.0, c = 0.0;
    double start = omp_get_wtime();

#pragma omp parallel for reduction(+ : c)
    for (long long int i = 0; i < num_operations; i++)
    {
        c += a * b + a / b; // 2 floating ops
    }

    double end = omp_get_wtime();
    double time = end - start;
    double flops = (2.0 * num_operations) / time;

    printf("Time = %f seconds\n", time);
    printf("Approx. FLOPS = %e\n", flops);
    return 0;
}
