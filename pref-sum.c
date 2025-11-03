// Sequential version

#include <stdio.h>
#include <omp.h>

#define N 8

int main()
{
    int A[N], P[N];
    for (int i = 0; i < N; i++)
        A[i] = i + 1;

    double start = omp_get_wtime();

    // Sequential prefix sum
    P[0] = A[0];
    for (int i = 1; i < N; i++)
        P[i] = P[i - 1] + A[i];

    double end = omp_get_wtime();

    printf("Sequential prefix sum:\n");
    for (int i = 0; i < N; i++)
        printf("%d ", P[i]);
    printf("\nTime: %f\n", end - start);
}

// Parallel version

#include <stdio.h>
#include <omp.h>

int main()
{
    int n = 8;
    int A[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int prefix[8];
    int i;

    prefix[0] = A[0];

#pragma omp parallel for
    for (i = 1; i < n; i++)
    {
        prefix[i] = 0;
        for (int j = 0; j <= i; j++)
            prefix[i] += A[j];
    }

    printf("Prefix Sum:\n");
    for (i = 0; i < n; i++)
        printf("%d ", prefix[i]);
    printf("\n");

    return 0;
}
