#include <stdio.h>
#include <omp.h>

#define N 5   // number of items

int main() {
    int buffer[N];
    int count = 0;  // how many items in buffer

    #pragma omp parallel sections shared(buffer, count)
    {
        // Producer section
        #pragma omp section
        {
            for (int i = 0; i < N; i++) {
                #pragma omp critical
                {
                    buffer[count] = i + 1;
                    printf("Produced: %d\n", buffer[count]);
                    count++;
                }
                #pragma omp flush(count)
            }
        }

        // Consumer section
        #pragma omp section
        {
            int consumed = 0;
            while (consumed < N) {
                #pragma omp critical
                {
                    if (count > 0) {
                        int item = buffer[count - 1];
                        count--;
                        printf("Consumed: %d\n", item);
                        consumed++;
                    }
                }
                #pragma omp flush(count)
            }
        }
    }

    return 0;
}
