#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[N];
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 100;

    printf("Original Array:\n");
    printArray(arr, N);

    double start = omp_get_wtime();

    for (int phase = 0; phase < N; phase++) {
        if (phase % 2 == 0) {
            // Even phase: compare (0,1), (2,3), (4,5), ...
            #pragma omp parallel for
            for (int i = 0; i < N - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    int temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }
        } else {
            // Odd phase: compare (1,2), (3,4), (5,6), ...
            #pragma omp parallel for
            for (int i = 1; i < N - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    int temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                }
            }
        }
    }

    double end = omp_get_wtime();

    printf("\nSorted Array:\n");
    printArray(arr, N);
    printf("\nExecution Time: %f seconds\n", end - start);

    return 0;
}
