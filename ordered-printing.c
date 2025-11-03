#include <stdio.h>
#include <omp.h>

#define N 10

int main() {
    int i;

    printf("Parallel Ordered Printing using OpenMP\n");
    printf("--------------------------------------\n");

    #pragma omp parallel for ordered
    for (i = 1; i <= N; i++) {
        // Simulate some computation
        int thread_id = omp_get_thread_num();
        double result = i * 2.5;

        // Ensure printing happens in order
        #pragma omp ordered
        {
            printf("Thread %d -> i = %d, result = %.2f\n", thread_id, i, result);
        }
    }

    return 0;
}
