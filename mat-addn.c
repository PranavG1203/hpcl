#include <stdio.h>
#include <omp.h>

int main()
{
    int N = 500;

    int A[N][N], B[N][N], C[N][N];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            A[i][j] = 1;
            B[i][j] = 2;
            C[i][j] = 0;
        }

    double start = omp_get_wtime();

#pragma omp parallel for shared(A, B, C)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    double end = omp_get_wtime();
    printf("Time taken: %f\n", end - start);

    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N; j++)
    //     {
    //         printf("%d ", C[i][j]);
    //     }
    //     printf("\n");
    // }
    return 0;
}